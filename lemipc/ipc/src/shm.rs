use super::*;
use std::fmt;
use std::mem::MaybeUninit;
use std::ptr;

pub struct IPCShm {
    // Идентификатор сегмента разделяемой памяти System V, связанный со значением ключа.
    id: i32,
    // Указатель на разделяемый участок памяти.
    pub ptr: *mut u8,
    // Размер разделяемого сегмента памяти.
    pub(super) size: usize,
    // Структура, где будет хранится информация о разделяемом сегменте памяти.
    // shm_nattch - количество текущих подключений сегмента разделяемой памяти.
    shmid_ds: libc::shmid_ds,
    pub(super) key: IPCkey,
}

impl<'a> IPCShm {
    /// Allocates a System V shared memory segment and returns it's ID
    pub(super) fn get(&mut self, size: usize, flg: i32) -> Result<(), BoxError> {
        unsafe {
            self.id = shmget(self.key.id, size, flg);
        }

        if self.id == -1 {
            return Err(Box::new(Error::last_os_error()));
        }

        Ok(())
    }

    /// Attaches the System V shared memory segment identified by shmid to the address space of the calling process.
    /// The attachment address is the address of the beginning of the shared memory
    pub fn join(&mut self) -> Result<*mut u8, BoxError> {
        let ptr = unsafe { shmat(self.id, ptr::null(), 0) };

        // error case
        if ptr::eq(ptr, usize::MAX as *mut c_void) == true {
            return Err(Box::new(Error::last_os_error()));
        }

        // save ptr in self.ptr
        self.ptr = ptr as *mut u8;

        Ok(ptr as *mut u8)
    }

    /// Detaches the shared memory segment located at the address specified by ptr
    /// from the address space of the calling process.
    pub fn detache(&mut self) -> Result<(), BoxError> {
        if let -1 = unsafe { shmdt(self.ptr as *mut libc::c_void) } {
            return Err(Box::new(Error::last_os_error()));
        }
        Ok(())
    }

    /// Получить количество текущих подключений к сегменту общей памяти
    pub fn njoined(&mut self) -> Result<u64, BoxError> {
        if let -1 = unsafe { shmctl(self.id, IPC_STAT, &mut self.shmid_ds) } {
            return Err(Box::new(Error::last_os_error()));
        }
        Ok(self.shmid_ds.shm_nattch.into())
    }
}

impl<'a> Default for IPCShm {
    fn default() -> Self {
        IPCShm {
            ptr: ptr::null_mut(),
            size: 0,
            id: 0,
            key: IPCkey::default(),
            shmid_ds: unsafe { MaybeUninit::zeroed().assume_init() },
        }
    }
}

impl<'a> fmt::Debug for IPCShm {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(
            f,
            "\n{{\n\tId: {}\n\tPointer: {:?}\n\tKey: {:?}\n\tSize: {}\n}}",
            self.id, self.ptr, self.key, self.size
        )
    }
}

// // TODO
// pub fn clean(&mut self) -> Result<(), BoxError> {
//     println!("We're the last process in shared memory, destroying it...");
//     self.key.clean();
//     if let -1 = unsafe { shmctl(self.id, IPC_RMID, ptr::null_mut()) } {
//         return Err(Box::new(Error::last_os_error()));
//     }
//     Ok(())
// }
