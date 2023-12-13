// #![allow(warnings, unused)]

const KEY_FILE_SEM: &str = "/tmp/key_sem_R";
const KEY_FILE_SHM: &str = "/tmp/key_shm_R";
const KEY_FILE_MSG: &str = "/tmp/key_msg_R";
const PROJ_ID: i32 = 'R' as i32;

use ipc::{prelude::*, BoxError};
// use libc::abs;
// use std::{cmp::Ordering, process::Command, thread, time};
use std::{process::Command, thread, time};

mod action;
mod game;

use game::Game;

fn main() {
    let res = init();
    let (mut ipc, game) = match res {
        Ok((ipc, game)) => (ipc, game),
        Err(e) => {
            println!("{}", e);
            return;
        }
    };

    // DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE
    ipc.sem.lock().unwrap();
    // println!("THE IPC IS:\n{:#?}\n\n", ipc);
    // println!("THE GAME IS:\n{:#?}\n\n", game);
    println!("p: {} t: {}, uid_msg: {}", game.id_player, game.id_team, game.uid_msg);
    ipc.sem.unlock().unwrap();
    // DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE

    // DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE
    let time = time::Duration::from_secs(2);
    thread::sleep(time);
    // DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE DELETE
    action::play(game, ipc).unwrap();
}

fn init() -> Result<(IPC, Game), BoxError> {
    // парсинг параметров игры.
    let mut game = match Game::from_args() {
        Ok(game) => game,
        Err(e) => {
            return Err(e);
        }
    };

    // создание или подключение к System V IPC
    let mut ipc = match IPC::new(game.ipc_mod)
        .set(IPCType::Semaphore, KEY_FILE_SEM, PROJ_ID)
        .set(IPCType::Message, KEY_FILE_MSG, PROJ_ID)
        .set(IPCType::SharedMemory, KEY_FILE_SHM, PROJ_ID)
        .memory_size((game.map.x * game.map.y) as usize)
        .build()
    {
        Ok(ipc) => ipc,
        Err(e) => {
            return Err(e);
        }
    };

    // copy pointer to shared memory
    game.map.ptr = ipc.shm.ptr;

    // TODO: размещение игроков (пока что захордкожено)
    ipc.sem.lock().unwrap();
    game.station();
    ipc.sem.unlock().unwrap();

    // если процесс - инициализатор игры, то создаем дочерние процессы
    if let IPCInitOption::Create = ipc.ipc_mod {
        init_players(&game);
        game.map.print();
    }

    Ok((ipc, game))
}

// Функция для процесса-инициализатора игры, создает дочерние процессы.
fn init_players(game: &Game) {
    let mut flag = 0;

    for n in 1..=game.qt_teams {
        for i in 1..=game.qt_players {
            // skip initializer process
            if game.ipc_mod == IPCInitOption::Create && flag == 0 {
                flag = 1;
                continue;
            }

            let _res = Command::new("target/debug/lem-ipc")
                .arg(game.qt_players.to_string())
                .arg(game.qt_teams.to_string())
                .arg(game.map.x.to_string())
                .arg(game.map.y.to_string())
                .arg(i.to_string()) // id team
                .arg(n.to_string()) // id player
                .spawn()
                .expect("E! Failed to execute process");
        }
    }
}

// fn init_game(game: &mut Game, ipc: &mut IPC) {
//     if game.comm.id_msg == 1 {
//         if let Err(s) = game.comm.send(
//             CommTypeOutgoing::Turn(TurnTypeOutgoing::Myself),
//             ipc,
//             IPC_NOWAIT,
//         ) {
//             eprintln!("E!: {:?}", s);
//             return;
//         }
//     }

//     // TODO
//     // размещение игроков (пока что захордкожено)
//     ipc.sem.lock();
//     game.station();
//     ipc.sem.unlock();
// }

// fn dgb_hello(game: &mut Game, ipc: &mut IPC) {
//     let stime = time::Duration::from_secs(1);
//     thread::sleep(stime);

//     eprintln!(
//         "--------------------------------\nID: {} TEAM: {} PLAYER: {}\n--------------------------------\n",
//         game.comm.id_msg,
//         game.id_player,
//         game.id_team, //game.comm, ipc
//     );

//     let stime = time::Duration::from_secs(1);
//     thread::sleep(stime);
// }

// TEST TEST TEST TEST TEST
// if let IPCInitOption::Create = ipc.ipc_mod {
//     unsafe {
//         // *ipc.shm.ptr.offset(15) = b'x';
//         // *ipc.shm.ptr.offset(16) = b'x';
//         // *ipc.shm.ptr.offset(17) = b'x';
//         // *ipc.shm.ptr.offset(17) = b'x';
//         // *ipc.shm.ptr.offset(18) = b'x';
//     }
// }
