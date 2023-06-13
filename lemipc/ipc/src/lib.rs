pub mod key;
pub mod msg;
pub mod sem;
pub mod shm;
pub mod prelude {
    pub const IPC_WAIT: i32 = 0;
    pub use crate::msg::Msg;
    pub use libc::ENOMSG as IPC_ENOMSG;
    pub use libc::{IPC_CREAT, IPC_EXCL, IPC_NOWAIT, IPC_RMID, IPC_STAT};
    pub use super::{IPC, IPCInitOption, IPCType};
}

extern "C" {
    fn msgget(key: i32, msgflg: c_int) -> c_int;
    fn msgrcv(msqid: c_int, msgp: *mut c_void, msgsz: size_t, msgtyp: c_long, msgflg: c_int) -> ssize_t;
    fn msgsnd(msqid: c_int, msgp: *const c_void, msgsz: size_t, msgflg: c_int) -> c_int;
    // fn msgctl(msqid: c_int, cmd: c_int, buf: *mut u8) -> c_int;
}

use libc::{c_int, c_long, c_void, size_t, ssize_t};
use libc::{ftok, semctl, semget, semop, shmat, shmctl, shmdt, shmget};
use std::io::Error;

use key::IPCkey;
use msg::IPCMsg;
use prelude::*;
use sem::IPCSem;
use shm::IPCShm;

pub type BoxError = Box<dyn std::error::Error>;

const FLAG_SEMAPHORE: u8 = 1 << 0;
const FLAG_SHARED_MEM: u8 = 1 << 1;
const FLAG_MESSAGE: u8 = 1 << 2;

#[derive(Debug)]
pub struct IPC {
    pub shm: IPCShm,
    pub sem: IPCSem,
    pub msg: IPCMsg,
    ipc_flags: u8,
    pub ipc_mod: IPCInitOption,
}

#[derive(Default, Debug, Clone, Copy, PartialEq)]
pub enum IPCInitOption {
    Create,
    #[default]
    Join,
}

#[derive(PartialEq)]
pub enum IPCType {
    SharedMemory,
    Semaphore,
    Message,
}

impl<'a> IPC {
    pub fn new(ipc_mod: IPCInitOption) -> Self {
        IPC {
            shm: Default::default(),
            sem: Default::default(),
            msg: IPCMsg::default(),
            ipc_flags: Default::default(),
            ipc_mod: ipc_mod,
        }
    }

    pub fn set(mut self, ipc_type: IPCType, path: &str, id_project: i32) -> Self {
        let closure_set_path = |key: &mut IPCkey| {
            key.set_path(path);
            key.set_project_id(id_project);
        };

        let flag = match ipc_type {
            IPCType::Message => {
                closure_set_path(&mut self.msg.key);
                FLAG_MESSAGE
            }
            IPCType::SharedMemory => {
                closure_set_path(&mut self.shm.key);
                FLAG_SHARED_MEM
            }
            IPCType::Semaphore => {
                closure_set_path(&mut self.sem.key);
                FLAG_SEMAPHORE
            }
        };

        // Заполняем битовую маску
        self.ipc_flags = self.ipc_flags | flag;
        self
    }

    pub fn memory_size(mut self, size: usize) -> Self {
        self.shm.size = size;
        self
    }

    pub fn build(mut self) -> Result<Self, BoxError> {
        // IPCType::SharedMemory должен быть обязательно установлен через set().
        // IPCType::Semaphore и IPCType::Message опциональны
        if self.ipc_flags & FLAG_SHARED_MEM == 0 {
            return Err("E! Using of shared memory is necessarily! Please, add needed type".into());
        }

        // Выставляем флаги прав доступа и др. для IPC функций
        let flags = match self.ipc_mod {
            IPCInitOption::Create => IPC_EXCL | IPC_CREAT | 0o666,
            IPCInitOption::Join => 0o666,
        };

        // 1. Семафоры.
        if self.ipc_flags & FLAG_SEMAPHORE != 0 {
            // Генерация ключа.
            if let Err(e) = self.sem.key.keygen(&self.ipc_mod) {
                eprintln!("E! SEMAPHORE: Key is bad!");
                return Err(e);
            }

            // Создание набора семафоров и/или получение его идентификатора.
            if let Err(e) = self.sem.get(flags) {
                eprintln!("E! SEMAPHORE: Couldn't create set or get his ID!");
                return Err(e);
            }

            // Если процесс - инициализатор, то выставляем количество доступных семафоров: 0.
            // Нужно для того, чтобы 'обезопасить' процесс инициализации семафоров, сегмента памяти, очереди сообщений от других процессов.
            if let IPCInitOption::Create = self.ipc_mod {
                if let Err(e) = self.sem.init_count(0) {
                    eprintln!("E! SEMAPHORE: Failed to change the number of available semaphores (0)");
                    return Err(e);
                }
            }
        }

        // 2. Сегмент разделяемой памяти.
        // Генерация ключа.
        if let Err(e) = self.shm.key.keygen(&self.ipc_mod) {
            eprintln!("E! SHARED MEMORY: Key is bad!");
            return Err(e);
        }

        // Создание сегмента разделяемой памяти и/или получение его идентификатора.
        if let Err(e) = self.shm.get(self.shm.size, flags) {
            eprintln!("E! SHARED MEMORY: Couldn't create segment or get his ID! {}, {}", self.shm.size, flags);
            return Err(e);
        }

        // 'Подключение' к сегменту разделяемой памяти. (Получаем указатель).
        if let Err(e) = self.shm.join() {
            eprintln!("E! SHARED MEMORY: Couldn't join to segment!");
            return Err(e);
        }

        // 3. Очередь сообщений.
        if self.ipc_flags & FLAG_MESSAGE != 0 {
            // Генерация ключа.
            if let Err(e) = self.msg.key.keygen(&self.ipc_mod) {
                eprintln!("E! MESSAGE QUEUE: Key is bad!");
                return Err(e);
            }
            // Создание очереди сообщений и/или получение его идентификатора.
            if let Err(e) = self.msg.get(flags) {
                eprintln!("E! MESSAGE QUEUE: Couldn't create set or get his ID!");
                return Err(e);
            }
        }

        // Если процесс - инициализатор, то выставляем количество доступных семафоров: 1.
        // Процесс инициализации закончен.
        if self.ipc_mod == IPCInitOption::Create && self.ipc_flags & FLAG_SEMAPHORE != 0 {
            if let Err(e) = self.sem.init_count(1) {
                eprintln!("E! SEMAPHORE: Failed to change the number of available semaphores (0)");
                return Err(e);
            }
        }

        Ok(self)
    }
}

// impl Drop for IPC {
//     fn drop(&mut self) {
//         let mut is_shm_destroyed = 0;

//         self.sem.lock();

//         if self.shm.njoined().unwrap() == 1 {
//             self.shm.clean().unwrap();
//             self.msg.drop();
//             is_shm_destroyed = 1;
//         } else {
//             self.shm.detache().unwrap();
//         }

//         self.sem.unlock();
//         if is_shm_destroyed == 1 {
//             self.sem.clean();
//         }
//     }
// }

// use libc::{msgctl, msgget, msgrcv, msgsnd};

// #[repr(i32)]
// pub enum IPCFlg {
//     Excl = libc::IPC_EXCL,
//     Creat = libc::IPC_CREAT,
//     Rmid = libc::IPC_RMID,
//     Stat = libc::IPC_STAT,
// }
