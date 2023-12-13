use super::*;

#[derive(Debug, PartialEq, Copy, Clone)]
pub struct Position {
    pub sender_uid: u8,
    pub receiver_uid: u8,
    pub team_id: u64,
    pub coord: u64,
}

#[derive(Debug, PartialEq)]
pub struct FindWay {
    pub sender_uid: u8,
    pub receiver_uid: u8,
    pub ally: Option<FWData>,
    pub foe: Option<FWData>,
}

#[derive(Debug, PartialEq, Default)]
pub struct FWData {
    pub uid: u8,
    pub coord: u16,
    pub move_qt: u64, // количество ходов до координаты в этой структуре
}

impl Serialize for FWData {
    fn serialize(&self, buf: &mut impl Write) -> io::Result<usize> {
        buf.write_u8(self.uid)?;
        buf.write_u16::<BigEndian>(self.coord)?;
        buf.write_u64::<BigEndian>(self.move_qt)?;
        Ok(TODO_LATER)
    }
}
impl Deserialize for FWData {
    type Output = FWData;

    fn deserialize(buf: &mut impl Read) -> io::Result<Self::Output> {
        let uid = buf.read_u8()?;
        let coord = buf.read_u16::<BigEndian>()?;
        let move_qt = buf.read_u64::<BigEndian>()?;

        Ok(FWData {
            uid,
            coord,
            move_qt,
        })
    }
}
