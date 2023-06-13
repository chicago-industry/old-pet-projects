#![allow(dead_code, unused_variables)]
use super::{BoxError, Game, IPC};
use ipc::prelude::*;
use lemipc_serde::{request, response, Request, Response, Serialize};

pub(super) struct All<'a> {
    pub ipc: IPC,
    pub game: Game,
    pub msg: &'a mut Msg<[u8]>,
    pub msg_recv: &'a mut Msg<[u8]>,
}

impl<'a> All<'a> {
    pub fn new(ipc: IPC, game: Game, msg: &'a mut Msg<[u8]>, msg_recv: &'a mut Msg<[u8]>) -> Self {
        All {
            ipc,
            game,
            msg,
            msg_recv,
        }
    }

    pub fn send_req_findway(&mut self, receiver: u8, positions: &Vec<response::Position>, msg_flag: i32) -> Result<(), BoxError> {
        // let req = Request::FindWay(request::FindWay {
        //     sender_uid: self.game.uid_msg,
        //     receiver_uid: receiver,
        //     ally: ally,
        //     foe: foe,
        // });

        // ПЕРЕД ТЕМ КАК ОТПРАВЛЯТЬ, НУЖНО ОПРЕДЕЛИТСЯ, КОМУ ОТПРАВЛЯТЬ
        // 1 НУЖНО САМОСТОЯТЕЛЬНО НАЙТИ БЛИЖАЙЩЕГО СОЮЗНИКА

        let (ally, foe): (Vec<response::Position>, Vec<response::Position>) = positions
            .into_iter()
            .partition(|data_positon| data_positon.team_id == self.game.id_team as u64);

        println!("{} JUST FOR TESTING {:#?}\n{:#?}", self.game.uid_msg, ally, foe);

        // self.msg.mtype = receiver as i64;
        // req.serialize(&mut self.msg.mbuf.as_mut())?;
        // self.ipc.msg.send::<[u8]>(&self.msg, msg_flag)?;
        Ok(())
    }

    pub fn send_req_turn(&mut self, receiver: u8, msg_flag: i32) -> Result<(), BoxError> {
        let req = Request::Turn(request::Turn {
            sender_uid: self.game.uid_msg,
            receiver_uid: receiver,
        });

        self.msg.mtype = receiver as i64;
        req.serialize(&mut self.msg.mbuf.as_mut())?;
        self.ipc.msg.send::<[u8]>(&self.msg, msg_flag)?;
        Ok(())
    }

    pub fn send_req_position_to_all(&mut self) -> Result<(), BoxError> {
        let uids = self.game.get_alive_players_uids();

        uids.iter().try_for_each(|uid| self.send_req_position(*uid as u8, IPC_NOWAIT))
    }

    pub fn send_req_position(&mut self, receiver: u8, msg_flag: i32) -> Result<(), BoxError> {
        let req = Request::Position(request::Position {
            sender_uid: self.game.uid_msg,
            receiver_uid: receiver,
        });

        self.msg.mtype = receiver as i64;
        req.serialize(&mut self.msg.mbuf.as_mut())?;
        self.ipc.msg.send::<[u8]>(&self.msg, msg_flag)?;
        Ok(())
    }

    pub fn send_resp_position(&mut self, receiver: u8, msg_flag: i32) -> Result<(), BoxError> {
        let resp = Response::Position(response::Position {
            sender_uid: self.game.uid_msg,
            receiver_uid: receiver,
            team_id: self.game.id_team as u64,
            coord: self.game.map.coord as u64,
        });

        self.msg.mtype = receiver as i64;
        resp.serialize(&mut self.msg.mbuf.as_mut())?;
        self.ipc.msg.send::<[u8]>(&self.msg, msg_flag)?;
        Ok(())
    }
}
