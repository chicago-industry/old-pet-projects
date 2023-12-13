use super::*;

#[derive(Debug, PartialEq)]
pub struct Turn {
    pub sender_uid: u8,
    pub receiver_uid: u8,
}

#[derive(Debug, PartialEq)]
pub struct Position {
    pub sender_uid: u8,
    pub receiver_uid: u8,
}

#[derive(Debug, PartialEq)]
pub struct FindWay {
    pub sender_uid: u8,
    pub receiver_uid: u8,
    // Пересылаем вектор игроков с их данными для расчета количества ходов
    // к каждому и определения ближайщих противника и союзника
    pub ally: Vec<FWData>, // отсылаем всех, кроме него самого
    pub foe: Vec<FWData>,
}

#[derive(Debug, PartialEq)]
pub struct FWData {
    pub uid: u8,
    pub coord: u16,
}

impl Serialize for FWData {
    fn serialize(&self, buf: &mut impl Write) -> io::Result<usize> {
        buf.write_u8(self.uid)?;
        buf.write_u16::<BigEndian>(self.coord)?;
        Ok(TODO_LATER)
    }
}

impl Deserialize for FWData {
    type Output = FWData;

    fn deserialize(buf: &mut impl Read) -> io::Result<Self::Output> {
        let uid = buf.read_u8()?;
        let coord = buf.read_u16::<BigEndian>()?;
        Ok(FWData { uid, coord })
    }
}
