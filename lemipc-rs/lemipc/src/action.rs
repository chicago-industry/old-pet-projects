mod logic;
mod state;
extern crate ipc;
extern crate lemipc_serde;

use super::{BoxError, Game, IPCInitOption, IPC};
use ipc::prelude::*;
use logic::All;
use state::State;
use std::{thread, time};

// задача сделать так, чтобы все события обрабатывались в основном цикле
// но могут приходить данные по кускам, нужно их как-то через enum запоминать

pub fn play(game: Game, ipc: IPC) -> Result<(), BoxError> {
    let mut msg: Box<Msg<[u8]>> = Box::new(Msg {
        mtype: 0,
        mbuf: [0; 500],
    });

    let mut msg_recv: Box<Msg<[u8]>> = Box::new(Msg {
        mtype: 0,
        mbuf: [0; 500],
    });

    let mut all = All::new(ipc, game, msg.as_mut(), msg_recv.as_mut());
    let mut state = State::default();

    // player-initializer have to send first message to himself to start the game
    if let IPCInitOption::Create = all.ipc.ipc_mod {
        // state_send = SendState::Send;
        all.send_req_turn(all.game.uid_msg, IPC_NOWAIT)?;
        println!("i'm initializer! {:?}", state);
    }

    loop {
        sleep_2_secs();

        let result = all.ipc.msg.recv_static(all.msg_recv, all.game.uid_msg.into(), IPC_NOWAIT);

        match result {
            Err(e) if e == IPC_ENOMSG => {
                println!("There is no message");
            }
            Err(e) => {
                return Err(format!("E! msgrcv() got error! Error num is: {}", e).into());
            }
            Ok(_) => {
                state.outer_transition(&mut all)?;
                all.msg_recv.mbuf.fill(0);
            }
        }
    }
}

// DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE
// KIRILL
fn sleep_2_secs() {
    let time = time::Duration::from_secs(2);
    thread::sleep(time);
}
// DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE
