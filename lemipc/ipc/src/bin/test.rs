use std::fs;
use std::str;
use ipc::prelude::*;

const KSHM: &str = "/tmp/key_shm_R";
const KMSG: &str = "/tmp/key_msg_R";
const PID: i32 = 'R' as i32;

fn main() {
    let _ = fs::remove_file(KSHM);
    let _ = fs::remove_file(KMSG);

    let ipc = match IPC::new(IPCInitOption::Create)
        .set(IPCType::SharedMemory, KSHM, PID)
        .set(IPCType::Message, KMSG, PID)
        .memory_size(1)
        .build()
    {
        Ok(ipc) => ipc,
        Err(e) => {
            eprintln!("{}", e);
            return;
        }
    };

    let mut msg = Box::new(Msg {
        mtype: 2,
        mbuf: [0;100],
    });

    msg.mbuf[0] = 100;
    msg.mbuf[1] = 102;
    msg.mbuf[2] = 103;

    println!("SENDING:\n{:?}", msg);

    ipc.msg.send::<[u8]>(msg.as_ref(), IPC_NOWAIT).unwrap();

    let mut l: Box<Msg<[u8]>> = Box::new(Msg {
        mtype: 2,
        mbuf: [0; 100],
    });

    let _ = ipc.msg.recv_static(&mut l.as_mut(), 2, IPC_WAIT);

    println!("{:#?}", ipc);
    println!("RECEIVED:\n{:?}", l);

}
