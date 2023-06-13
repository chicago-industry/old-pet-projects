use super::*;

#[repr(C)]
#[derive(Debug)]
pub struct Msg<T: ?Sized> {
    pub mtype: i64,
    pub mbuf: T,
}

// impl <T> Msg<T> {
//     pub fn new(size: usize) -> Box<Self> {
//         let msg = Box::new(Msg {
//             mtype: 0,
//             mbuf: [0; size],
//         });
//         msg
//     }
// }

#[derive(Default, Debug)]
pub struct IPCMsg {
    id: i32, // идентификатор очереди сообщений System V, связанный со значением ключа
    pub(super) key: IPCkey,
}

impl<'a> IPCMsg {
    // pub fn send<T: ?Sized>(&self, msg: Box<Msg<[u8]>>, msg_flag: i32) -> Result<(), BoxError> {
    pub fn send<T: ?Sized>(&self, msg: &Msg<[u8]>, msg_flag: i32) -> Result<(), BoxError> {
        let size = msg.mbuf.len();
        let ptr = msg as *const _ as *const c_void;

        unsafe {
            if let -1 = msgsnd(self.id, ptr, size, msg_flag) {
                return Err(Box::new(Error::last_os_error())); // No thread safe???
            }
        }

        Ok(())
    }

    pub fn recv_static(&self, buf: & mut Msg<[u8]>, mtype: i64, msg_flag: i32) -> Result<isize, i32> {
        let size = buf.mbuf.len();
        let ptr = buf as *mut _ as *mut c_void;

        let i = unsafe { msgrcv(self.id, ptr, size, mtype, msg_flag) };

        if i == -1 {
            let e = Error::last_os_error().raw_os_error().unwrap_or(-1);
            return Err(e)
        }
        Ok(i)
    }

    // pub fn recv_dynamic(&mut self) {
    // }

    /// get a System V message queue identifier
    pub(super) fn get(&mut self, flg: i32) -> Result<(), BoxError> {
        unsafe {
            self.id = msgget(self.key.id, flg);
        }

        if self.id == -1 {
            return Err(Box::new(Error::last_os_error()));
        }

        Ok(())
    }
}
