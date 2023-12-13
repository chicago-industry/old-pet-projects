use byteorder::{BigEndian, ReadBytesExt, WriteBytesExt};
use std::io::{self, Read, Write};

pub mod request;
pub mod response;

const TODO_LATER: usize = 177;

// Перевод структуры данных в битовую последовательность
pub trait Serialize {
    fn serialize(&self, buf: &mut impl Write) -> io::Result<usize>;
}

// Создание структуры данных из битовой последовательности
pub trait Deserialize {
    type Output;
    fn deserialize(buf: &mut impl Read) -> io::Result<Self::Output>;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
// REQUEST                                                                                                            //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Turn, Position
// | u8   | u8         | u8           |
// |______|____________|______________|
// | type | byte       | byte         |
// |______|____________|______________|
// |      | sender uid | receiver uid |

// FindWay
// | u8   | u8         | u8           | u8              | ...[(u8, u16),...] | u8            | ...[(u8, u16),...] |
// |______|____________|______________|_________________|____________________|_______________|____________________|
// | type | byte       | byte         | byte            | bytes              | byte          | bytes              |
// |______|____________|______________|_________________|____________________|_______________|____________________|
// |      | sender uid | receiver uid | count of allies | ally: uid, coord   | count of foes | foe: uid, coord    |

#[derive(Debug, PartialEq)]
pub enum Request {
    // Передача хода
    Turn(request::Turn),
    // Запрос позиции
    Position(request::Position),
    // Запрос союзника рассчитать и определить ближайщего противника и союзника от него самого
    FindWay(request::FindWay),
    // // Запрос проверки состояния
    // IsDead {
    //     sender_uid: u8,
    //     receiver_uid: u8,
    // },
}

impl From<&Request> for u8 {
    fn from(req: &Request) -> Self {
        match req {
            Request::Turn { .. } => 1,
            Request::Position { .. } => 2,
            Request::FindWay { .. } => 3,
        }
    }
}

impl Serialize for Request {
    fn serialize(&self, buf: &mut impl Write) -> io::Result<usize> {
        buf.write_u8(self.into())?; // message type

        match self {
            Request::Turn(x) => {
                buf.write_u8(x.sender_uid)?;
                buf.write_u8(x.receiver_uid)?;
            }
            Request::Position(x) => {
                buf.write_u8(x.sender_uid)?;
                buf.write_u8(x.receiver_uid)?;
            }
            Request::FindWay(x) => {
                buf.write_u8(x.sender_uid)?;
                buf.write_u8(x.receiver_uid)?;

                // Замыкание, принимает Vec<DataRq>, пишет в buf количество игроков, затем их позиции и координаты по порядку.
                let mut cl_write_players = |player: &Vec<request::FWData>| -> io::Result<()> {
                    buf.write_u8(player.len() as u8)?;

                    player.iter().try_for_each(|p| -> io::Result<()> {
                        p.serialize(buf)?;
                        Ok(())
                    })?;
                    Ok(())
                };

                cl_write_players(&x.ally)?;
                cl_write_players(&x.foe)?;
            }
        }
        Ok(TODO_LATER)
    }
}

impl Deserialize for Request {
    type Output = Request;

    fn deserialize(buf: &mut impl Read) -> io::Result<Self::Output> {
        let msg_type = buf.read_u8()?;
        let sender_uid = buf.read_u8()?;
        let receiver_uid = buf.read_u8()?;

        match msg_type {
            1 => Ok(Request::Turn(request::Turn {
                sender_uid,
                receiver_uid,
            })),
            2 => Ok(Request::Position(request::Position {
                sender_uid,
                receiver_uid,
            })),
            3 => Ok(Request::FindWay(request::FindWay {
                sender_uid,
                receiver_uid,
                ally: Request::parse_findway(buf)?,
                foe: Request::parse_findway(buf)?,
            })),
            // 3 => Ok(Request::IsDead {
            // sender_uid,
            // receiver_uid,
            // }),
            _ => Err(io::Error::new(
                io::ErrorKind::InvalidData,
                "Invalid Request Type",
            )),
        }
    }
}

impl Request {
    fn parse_findway(buf: &mut impl Read) -> io::Result<Vec<request::FWData>> {
        let count = buf.read_u8()?;

        let mut players = Vec::with_capacity(count.into());

        for _ in 0..count {
            players.push(request::FWData::deserialize(buf)?)
        }
        Ok(players)
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
// RESPONSE                                                                                                           //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Position
// | u8   | u8         | u8           | u64          |
// |______|____________|______________|______________|
// | type | byte       | byte         | bytes        |
// |______|____________|______________|______________|
// |      | sender uid | receiver uid | sender coord |

// FindWay
// | u8   | u8         | u8           | u8                            | ...[(u8, u16, u64),...]          | u8                            | ...[(u8, u16, u64),...]          |
// |______|____________|______________|_______________________________|__________________________________|_______________________________|__________________________________|
// | type | byte       | byte         | byte                          | bytes                            | byte                          | bytes                            |
// |______|____________|______________|_______________________________|__________________________________|_______________________________|__________________________________|
// |      | sender uid | receiver uid | is the a way to ally (1 or 0) | ally: uid, coord, count of moves | is the a way to foe (1 or 0)  | foe: uid, coord, count of moves  |

#[derive(Debug, PartialEq)]
pub enum Response {
    // Ответ на запрос о позиции
    Position(response::Position),
    // Ответ на запрос от союзника рассчитать и определить ближайщего
    // противника и союзника
    FindWay(response::FindWay),
}

impl From<&Response> for u8 {
    fn from(resp: &Response) -> Self {
        match resp {
            Response::Position { .. } => 1,
            Response::FindWay { .. } => 2,
        }
    }
}

impl Serialize for Response {
    fn serialize(&self, buf: &mut impl Write) -> io::Result<usize> {
        buf.write_u8(self.into())?; // message type
        match self {
            Response::Position(x) => {
                buf.write_u8(x.sender_uid)?;
                buf.write_u8(x.receiver_uid)?;
                buf.write_u64::<BigEndian>(x.team_id)?;
                buf.write_u64::<BigEndian>(x.coord)?;
            }
            Response::FindWay(x) => {
                buf.write_u8(x.sender_uid)?;
                buf.write_u8(x.receiver_uid)?;

                // Замыкание, принимает Option<DataRq>, пишет в buf количество игроков (всегда будет 1), затем их позиции и координаты по порядку.
                // Если Option<DataRq> - None, то это означает, что пути к игроку нету, в этом случае пишем 0.
                let mut cl_write_players = |player: &Option<response::FWData>| -> io::Result<()> {
                    if let Some(p) = player {
                        buf.write_u8(1 as u8)?;
                        p.serialize(buf)?;
                    } else {
                        buf.write_u8(0 as u8)?;
                    }
                    Ok(())
                };
                cl_write_players(&x.ally)?;
                cl_write_players(&x.foe)?;
            }
        }
        Ok(TODO_LATER)
    }
}

impl Deserialize for Response {
    type Output = Response;

    fn deserialize(buf: &mut impl Read) -> io::Result<Self::Output> {
        let msg_type = buf.read_u8()?;
        let sender_uid = buf.read_u8()?;
        let receiver_uid = buf.read_u8()?;

        match msg_type {
            1 => {
                let res = Response::Position(response::Position {
                    sender_uid,
                    receiver_uid,
                    team_id: buf.read_u64::<BigEndian>()?,
                    coord:  buf.read_u64::<BigEndian>()?,
                });

                Ok(res)
            }
            2 => {
                let res = Response::FindWay(response::FindWay {
                    sender_uid,
                    receiver_uid,
                    ally: Response::parse_findway(buf)?,
                    foe: Response::parse_findway(buf)?,
                });

                Ok(res)
            }
            _ => {
                return Err(io::Error::new(
                    io::ErrorKind::InvalidData,
                    "Invalid Request Type",
                ))
            }
        }
    }
}

impl Response {
    fn parse_findway(buf: &mut impl Read) -> io::Result<Option<response::FWData>> {
        let is_ally: u8 = buf.read_u8()?;

        if let 1 = is_ally {
            Ok(Some(response::FWData::deserialize(buf)?))
        } else {
            Ok(None)
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                    //
// TESTS                                                                                                              //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#[cfg(test)]
mod test {
    #![allow(unused_imports)]
    use super::*;
    use std::{fs::read, io::Cursor};

    #[test]
    // Request Turn, Position
    fn test_request_turn_position_roundtrip() {
        let req = Request::Turn(request::Turn {
            sender_uid: 1,
            receiver_uid: 1,
        });

        let mut bytes: Vec<u8> = vec![];
        req.serialize(&mut bytes).unwrap();

        let mut reader = Cursor::new(bytes);
        let roundtrip_req = Request::deserialize(&mut reader).unwrap();

        assert!(roundtrip_req == req);
    }

    #[test]
    // Request FindWay
    fn test_request_findway_roundtrip() {
        let req = Request::FindWay(request::FindWay {
            sender_uid: 4,
            receiver_uid: 5,
            ally: vec![
                request::FWData {
                    uid: 254,
                    coord: 600,
                },
                request::FWData { uid: 3, coord: 3 },
            ],
            foe: vec![request::FWData { uid: 4, coord: 4 }],
        });

        let mut bytes: Vec<u8> = vec![];
        req.serialize(&mut bytes).unwrap();

        let mut reader = Cursor::new(bytes);
        let roundtrip_req = Request::deserialize(&mut reader).unwrap();

        // println!("{:?}", reader);
        // println!("1: {:#?}", roundtrip_req);
        // println!("2: {:#?}", req);

        assert!(roundtrip_req == req);
    }

    #[test]
    // Response FindWay
    fn test_response_findway_roundtrip() {
        let resp = Response::FindWay(response::FindWay {
            sender_uid: 4,
            receiver_uid: 5,
            ally: Some(response::FWData {
                uid: 233,
                coord: 600,
                move_qt: 10,
            }),
            foe: Some(response::FWData {
                uid: 4,
                coord: 4,
                move_qt: 15,
            }),
        });

        let mut bytes: Vec<u8> = vec![];
        resp.serialize(&mut bytes).unwrap();

        let mut reader = Cursor::new(bytes);
        let roundtrip_resp = Response::deserialize(&mut reader).unwrap();

        // println!("{:?}", reader);
        // println!("1: {:#?}", roundtrip_resp);
        // println!("2: {:#?}", resp);

        assert!(roundtrip_resp == resp);
    }

    #[test]
    // Response Position
    fn test_response_position_roundtrip() {
        let resp = Response::Position(response::Position {
            sender_uid: 10,
            receiver_uid: 4,
            team_id: 2,
            coord: 255,
        });

        let mut bytes: Vec<u8> = vec![];
        resp.serialize(&mut bytes).unwrap();

        let mut reader = Cursor::new(bytes);
        let roundtrip_resp = Response::deserialize(&mut reader).unwrap();

        // println!("{:?}", reader);
        // println!("1: {:#?}", roundtrip_resp);
        // println!("2: {:#?}", resp);

        assert!(roundtrip_resp == resp);
    }
}
