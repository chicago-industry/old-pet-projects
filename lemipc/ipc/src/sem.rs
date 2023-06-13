use super::*;
use std::fmt;

#[cfg(target_os = "macos")]
const SETVAL: i32 = 8;
#[cfg(target_os = "linux")]
const SETVAL: i32 = 16;

pub struct IPCSem {
    // Идентификатор набора семафоров System V, связанный со значением ключа
    id: i32,
    // Структура для манипуляции элементов из набора семафоров
    // struct sembuf:
    // short sem_num;   /* semaphore number: 0 = first */
    // short sem_op;    /* semaphore operation */
    // short sem_flg;   /* operation flags */
    sembuf: libc::sembuf,
    // Количество семафоров в одном 'наборе'. Набор всегда один, лень делать... Да и не нужно...
    qt: i32,
    pub(super) key: IPCkey,
}

/// Stripped down version, only supports 1 set of semaphores
impl<'a> IPCSem {
    /// Get a System V semaphore set identifier (aviable only 1 set)
    pub(super) fn get(&mut self, flg: i32) -> Result<(), BoxError> {
        unsafe {
            self.id = semget(self.key.id, 1, flg);
        }

        if self.id == -1 {
            return Err(Box::new(Error::last_os_error()));
        }
        Ok(())
    }

    /// Lock the semaphore
    pub fn lock(&mut self) -> Result<(), BoxError> {
        self.op(-1)
    }

    /// Unlock the semaphore
    pub fn unlock(&mut self) -> Result<(), BoxError> {
        self.op(1)
    }

    /// Initialize created semaphore with starting value (>=0)
    pub fn init_count(&mut self, qt: i32) -> Result<(), BoxError> {
        self.qt = qt;

        if let -1 = unsafe { semctl(self.id, 0, SETVAL, qt) } {
            return Err(Box::new(Error::last_os_error()));
        }
        Ok(())
    }

    fn op(&mut self, val: i16) -> Result<(), BoxError> {
        self.sembuf.sem_op = val;

        if let -1 = unsafe { semop(self.id, &mut self.sembuf, 1) } {
            return Err(Box::new(Error::last_os_error()));
        }

        Ok(())
    }
}

impl<'a> Default for IPCSem {
    fn default() -> Self {
        IPCSem {
            id: 0,
            key: IPCkey::default(),
            sembuf: libc::sembuf {
                sem_num: 0,
                sem_op: 0,
                sem_flg: 0,
            },
            qt: -1,
        }
    }
}

impl<'a> fmt::Debug for IPCSem {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(
            f,
            "\n{{\n\tId: {}\n\tQuantity in set: {}\n\tSembuf struct: {{ sem_num: {}, sem_op: {}, sem_flg: {} }}\n\tKey: {:?}\n}}",
            self.id, self.qt, self.sembuf.sem_num, self.sembuf.sem_op, self.sembuf.sem_flg, self.key
        )
    }
}

// // TODO
// pub fn clean(&mut self) {
//     println!("We're the last process in shared memory, destroying it...");
//     self.key.clean();
//     unsafe {
//         semctl(self.id, IPC_RMID, 0);
//     }
// }
