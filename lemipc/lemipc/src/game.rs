#![allow(dead_code, unused_variables)]
mod args;
mod map;

use args::Args;
use ipc::IPCInitOption;
use map::Map;
use std::ptr;

macro_rules! ternary_op {
    ($cond:expr, $value:expr) => {
        if $cond {
            Some($value)
        } else {
            None
        }
    };
}

#[derive(Debug, Default)]
pub struct Game {
    // to define an initializer process
    pub ipc_mod: IPCInitOption,
    // количество команд
    pub qt_teams: u8,
    // количество игроков в команде
    pub qt_players: u8,
    // уникальное поле вместе с id_player
    pub id_team: u8,
    // уникальное поле вместе с id_team
    pub id_player: u8,
    // Вектор живых игроков, где индекс вектора - uid_msg для обмена сообщениями с другими игроками,
    // а значение по индексу - его статус (true - жив, false - мертв).
    pub alive: Vec<bool>,
    // uid для обмена сообщениями данного процеса.
    pub uid_msg: u8,
    pub map: Map,
}

impl Game {
    pub fn from_args() -> Result<Self, Box<dyn std::error::Error>> {
        let args = match Args::from_args() {
            Ok(args) => args,
            Err(e) => return Err(e),
        };

        let mut game = Game {
            ipc_mod: args.ipc_mod,
            qt_teams: args.qt_teams,
            qt_players: args.qt_players,
            id_team: args.id_team,
            id_player: args.id_player,
            map: Map::new(ptr::null_mut(), 0, args.x as usize, args.y as usize),
            ..Default::default()
        };

        game.alive = vec![true; (args.qt_players * args.qt_teams + 1).into()];
        // vec[0] - не учитывается и выставляется в false.
        game.alive[0] = false;
        // генерируем уникального индекса сообщения
        game.uid_msg = (((args.id_player - 1) * args.qt_players) + (args.id_team - 1) + 1) as u8;
        Ok(game)
    }

    pub fn next_player_uid_msg(&self) -> u8 {
        // [0] не используется
        let before = &self.alive[1..self.uid_msg as usize];
        let after = &self.alive[(self.uid_msg + 1) as usize..];

        let cl_before = |i, v| ternary_op!(v, i + 1);
        let cl_after = |i, v| ternary_op!(v, i + 1 + self.uid_msg as usize);

        // find someone's msg ID after and before myself
        if let Some(uid_msg) = after.iter().enumerate().find_map(|(i, v)| cl_after(i, *v)) {
            uid_msg as u8
        } else if let Some(uid_msg) = before.iter().enumerate().find_map(|(i, v)| cl_before(i, *v)) {
            uid_msg as u8
        } else {
            self.uid_msg
        }
    }

    // Количество живых игроков (исключая себя).
    pub fn get_alive_players_qt(&self) -> usize {
        self.alive.iter().filter(|&i| *i == true).count() - 1
    }

    // Возвращает вектор живых игроков, содержащий их uid. (исключая себя)
    pub fn get_alive_players_uids(&self) -> Vec<i64> {
        let cl_is_alive = |i, v| {
            if v && i != self.uid_msg as usize {
                Some(i as i64)
            } else {
                None
            }
        };

        self.alive
            .iter()
            .enumerate()
            .filter_map(|(i, v)| cl_is_alive(i, *v))
            .collect()
    }

    // TODO: размещение игроков (пока что захордкожено)
    pub fn station(&mut self) {
        unsafe {
            let mut pos: usize;

            if self.map.ptr.is_null() == true {
                panic!("E! Pointer to shared memory is NULL");
            }

            // 1 team must be at the top
            if self.id_team == 1 {
                pos = 0;
                if *self.map.ptr.offset(pos as isize) == 0 {
                    pos = 0;
                    *self.map.ptr.offset(pos as isize) = self.id_team;
                    self.map.coord = pos;
                } else {
                    pos = self.map.x - 1;
                    *self.map.ptr.offset(pos as isize) = self.id_team;
                    self.map.coord = pos;
                }
            }
            // 2 team must be at the down
            else {
                pos = self.map.x * self.map.y - self.map.x;

                if *self.map.ptr.offset(pos as isize) == 0 {
                    pos = self.map.x * self.map.y - self.map.x;
                    *self.map.ptr.offset(pos as isize) = self.id_team;
                    self.map.coord = pos;
                } else {
                    pos = self.map.x * self.map.y - 1;
                    *self.map.ptr.offset((pos) as isize) = self.id_team;
                    self.map.coord = pos;
                }
            }
        }
    }
}
