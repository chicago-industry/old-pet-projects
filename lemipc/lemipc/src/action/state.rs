#![allow(dead_code, unused_must_use)]

use super::ipc::prelude::*;
use super::lemipc_serde::{response, Deserialize, Request, Response};
use super::{All, BoxError};

#[derive(Default, Debug)]
pub enum OuterState {
    #[default]
    Request,
    Response(u8),
}

#[derive(Default, Debug)]
pub enum InnerState {
    #[default]
    Turn,
    Position,
    FindWay,
}

#[derive(Default, Debug)]
pub struct State {
    pub outer_state: OuterState,
    pub inner_state: InnerState,
    pub resp_positions: Vec<response::Position>,
}

impl State {
    fn inner_transition(&mut self, all: &mut All) -> Result<(), BoxError> {
        match self.inner_state {
            // получили ход
            // проверка, что игрок еще в игре
            // проверка наличия выживших игроков из враждующих команд
            // при условии, что количество враждующих команд может быть любым
            // запрос позиций у игроков
            InnerState::Turn => {
                all.send_req_position_to_all()?;

                self.inner_state = InnerState::Position;
                self.outer_state = OuterState::Response(all.game.get_alive_players_qt() as u8);
            }
            InnerState::Position => {
                println!("{} получил позиции от всех игроков! {:#?}\n передаю ход!", all.game.uid_msg, self.resp_positions);
                all.send_req_findway(22, &self.resp_positions, 1);
                self.resp_positions.clear();

                all.send_req_turn(all.game.next_player_uid_msg(), IPC_NOWAIT)?;

                self.inner_state = InnerState::Turn;
                self.outer_state = OuterState::Request;
            }
            InnerState::FindWay => {}
        }
        Ok(())
    }

    pub(super) fn outer_transition(&mut self, all: &mut All) -> Result<(), BoxError> {
        let mut buf = &all.msg_recv.mbuf[..];

        match self.outer_state {
            OuterState::Request => {
                let m = Request::deserialize(&mut buf)?;

                match m {
                    Request::Turn(_) => {
                        println!("{} мой ход, запрашиваю позиции!", all.game.uid_msg);
                        self.inner_transition(all)?;
                    }
                    Request::Position(data) => {
                        println!("{} у меня запросили позицию! {:?}", all.game.uid_msg, data);
                        all.send_resp_position(data.sender_uid, IPC_NOWAIT)?;
                    }
                    Request::FindWay(_) => {}
                }
            }

            OuterState::Response(ref mut remind) => {
                let m = Response::deserialize(&mut buf)?;

                match m {
                    Response::Position(data) => {
                        *remind = *remind - 1;
                        self.resp_positions.push(data);

                        if *remind == 0 {
                            self.inner_transition(all)?;
                        }
                    }
                    Response::FindWay(_) => {}
                }
            }
        }
        Ok(())
    }
}
