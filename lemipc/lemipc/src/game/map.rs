use std::char::from_digit;
use std::mem::size_of;
use std::ptr;
// use std::collections::HashSet;
// use std::hash::Hash;
// use std::{cell::RefCell, mem::size_of, ptr, rc::Rc};
// use std::{fmt, vec};
// use libc::Elf64_Section;
// use crate::action::play;
// use super::Game;
// mod astar;

#[derive(Debug)]
pub struct Map {
    pub ptr: *mut u8,
    pub x: usize,
    pub y: usize,
    pub coord: usize,
}

impl<'a> Default for Map {
    fn default() -> Self {
        Map {
            ptr: ptr::null_mut(),
            x: 0,
            y: 0,
            coord: 0,
        }
    }
}

#[derive(Debug)]
pub enum AreaWay {
    Left,
    Right,
    Up,
    Down,
    LeftUp,
    RightUp,
    LeftDown,
    RightDown,
}

impl Map {
    pub fn new(ptr: *mut u8, coord: usize, x: usize, y: usize) -> Self {
        Map { ptr, x, y, coord }
    }

    /// Deletes self player from the map
    pub fn erase(&mut self) {
        unsafe { *self.ptr.offset(self.coord as isize) = 0 };
    }

    /// Converts 1d coordinate to 2d
    pub fn from_1d_to_2d(&self, coord_1d: usize) -> (usize, usize) {
        (coord_1d % self.x, coord_1d / self.y)
    }

    /// Converts 2d coordinate to 1d
    pub fn from_2d_to_1d(&self, x: usize, y: usize) -> usize {
        y * self.y + x
    }

    /// calculates the index of the new position relative to the current one
    /// on a 2D map represented by a 1D array
    pub fn movement(&self, coord: usize, way: AreaWay) -> Option<usize> {
        let x = coord % self.x;
        let y = coord / self.x;

        match way {
            AreaWay::Up => {
                if y > 0 {
                    return Some(coord - self.x);
                }
            }
            AreaWay::Down => {
                if y < self.y - 1 {
                    return Some(coord + self.x);
                }
            }
            AreaWay::Left => {
                if x > 0 {
                    return Some(coord - 1);
                }
            }
            AreaWay::Right => {
                if x < self.x - 1 {
                    return Some(coord + 1);
                }
            }
            AreaWay::LeftUp => {
                if x > 0 && y > 0 {
                    return Some(coord - self.x - 1);
                }
            }
            AreaWay::LeftDown => {
                if x > 0 && y < self.y - 1 {
                    return Some(coord + self.x - 1);
                }
            }
            AreaWay::RightUp => {
                if x < self.x - 1 && y > 0 {
                    return Some(coord - self.x + 1);
                }
            }
            AreaWay::RightDown => {
                if x < self.x - 1 && y < self.y - 1 {
                    return Some(coord + self.x + 1);
                }
            }
        }
        None
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn movements() {
        let mut area = [
            // 1  2
            0, 1, 2, // 0
            3, 4, 5, // 1
            6, 7, 8, // 2
        ];
        let area_len = 3;
        let coord = 4;
        let mut map = Map::new(area.as_mut_ptr(), coord, area_len, area_len);

        assert!(Some(3) == map.movement(map.coord, AreaWay::Left));
        assert!(Some(5) == map.movement(map.coord, AreaWay::Right));
        assert!(Some(1) == map.movement(map.coord, AreaWay::Up));
        assert!(Some(7) == map.movement(map.coord, AreaWay::Down));
        assert!(Some(0) == map.movement(map.coord, AreaWay::LeftUp));
        assert!(Some(6) == map.movement(map.coord, AreaWay::LeftDown));
        assert!(Some(2) == map.movement(map.coord, AreaWay::RightUp));
        assert!(Some(8) == map.movement(map.coord, AreaWay::RightDown));

        map.coord = 8;

        assert!(Some(7) == map.movement(map.coord, AreaWay::Left));
        assert!(None == map.movement(map.coord, AreaWay::Right));
        assert!(Some(5) == map.movement(map.coord, AreaWay::Up));
        assert!(None == map.movement(map.coord, AreaWay::Down));
        assert!(Some(4) == map.movement(map.coord, AreaWay::LeftUp));
        assert!(None == map.movement(map.coord, AreaWay::LeftDown));
        assert!(None == map.movement(map.coord, AreaWay::RightUp));
        assert!(None == map.movement(map.coord, AreaWay::RightDown));
    }
}

// const MOV_ALL: [AreaWay; 8] = [
//     AreaWay::Left,
//     AreaWay::Right,
//     AreaWay::Up,
//     AreaWay::Down,
//     AreaWay::LeftUp,
//     AreaWay::RightUp,
//     AreaWay::LeftDown,
//     AreaWay::RightDown,
// ];

// const MOV_ORTHOGONALITY: [AreaWay; 4] = [
//     AreaWay::Left,
//     AreaWay::Right,
//     AreaWay::Up,
//     AreaWay::Down,
// ];

//     /// Changes the value of the player (id)
//     /// from one position to another on the map (contiguous area of memory).
//     pub fn shift(&mut self, new_coord: isize) {
//         assert!(!self.ptr.is_null());

//         unsafe {
//             if *self.ptr.offset(new_coord) != 0 {
//                 return;
//             }

//             let id = *self.ptr.offset(self.coord);
//             *self.ptr.offset(self.coord) = 0;
//             self.coord = new_coord;
//             *self.ptr.offset(self.coord) = id;
//         }
//     }

//     //
//     //
//     //
//     //
//     //
//     //
//     //
//     //

//     /// Проверяем way (влево/вправо/вниз/вверх/шаги по диагонали) относительно входящей координаты на карте.
//     /// Возвращаем координату и значение по координате (если значение > 0, то это id_team или препятствие), если данный шаг возможно сделать
//     fn get_data_by_way(&self, coord: isize, way: AreaWay) -> Option<Player> {
//         assert!(!self.ptr.is_null());

//         let coord = if let Some(coord) = Map::movement(self, coord, way) {
//             coord
//         } else {
//             return None;
//         };

//         let player = Player {
//             coord: coord,
//             id_team: unsafe { *self.ptr.offset(coord) as i64 },
//         };

//         Some(player)
//     }

//     /// Проверяем way (влево/вправо/вниз/вверх/шаги по диагонали) относительно входящей координаты.
//     /// Возвращаем координату и значение по координате (id_team, если не 0),
//     /// если данный шаг возможно сделать и значение по возвращаемой координате 'не пустое' (есть игрок).
//     fn get_data_by_coord_nonempty(&self, coord: isize, way: AreaWay) -> Option<Player> {
//         assert!(!self.ptr.is_null());

//         let coord = if let Some(coord) = Map::movement(self, coord, way) {
//             coord
//         } else {
//             return None;
//         };

//         let player = Player {
//             coord: coord,
//             id_team: unsafe { *self.ptr.offset(coord) as i64 },
//         };

//         (player.id_team != 0 && player.id_team != b'#' as i64).then_some(player)
//     }

//     /// Найти противников вокруг себя.
//     /// Возвращает вектор противников, которые находятся вокруг входящей координаты.
//     fn get_foe_data_around_coord(&self, coord: isize, id_team: i64) -> Option<Vec<Player>> {
//         // Максимальное количество точек вокруг игрока == MOV_ALL.len().
//         let mut foes = Vec::with_capacity(MOV_ALL.len());

//         for foe in MOV_ALL
//             .into_iter()
//             .filter_map(|way| self.get_data_by_coord_nonempty(coord, way))
//             .filter(|player| player.id_team != id_team)
//         {
//             foes.push(foe);
//         }

//         (foes.is_empty() == false).then_some(foes)
//     }

//     // Находит противников вокруг собственной координаты и проверяет свое состояние.
//     // Если вокруг находится >= 2 игроков из одинаковой чужой команды - то мы мертвы.
//     pub fn is_dead_myself(&self, id_team: i64) -> bool {
//         // Находим противников вокруг своей координаты.
//         let foes = if let Some(foes) = self.get_foe_data_around_coord(self.coord, id_team) {
//             foes
//         } else {
//             return false;
//         };

//         // Если есть совпадения по id_team (>= 2)
//         // в векторе противников - мертв.
//         !has_unique_elements(foes.iter())
//     }

//     // Находит противников вокруг заданной координаты и для каждого из найденных противников
//     // проверяет, что вокруг них не находится более 2-х игроков из других команд.
//     // Возвращает вектор координат мертвых игроков.
//     pub fn is_dead_foe_around(&self, coord: isize, id_team: i64) -> Option<Vec<isize>> {
//         // Находим противников вокруг заданной координаты.
//         let foes = self.get_foe_data_around_coord(coord, id_team)?;

//         // Для этих противников находим их противников (в т.ч. и себя).
//         let mut deads = Vec::with_capacity(MOV_ALL.len());

//         // Для каждого противника ищем вокруг них противников.
//         for foe in foes.into_iter().map(|f| {
//             (
//                 f,
//                 self.get_foe_data_around_coord(f.coord, f.id_team).unwrap(),
//             )
//         }) {
//             // Если есть совпадения по id_team (>= 2)
//             // в векторе противников для противника - он мертв.
//             if !has_unique_elements(foe.1.iter()) {
//                 deads.push(foe.0.coord);
//             }
//         }

//         (deads.is_empty() == false).then_some(deads)
//     }
// }

// fn has_unique_elements<T>(iter: T) -> bool
// where
//     T: IntoIterator,
//     T::Item: Eq + Hash,
// {
//     let mut uniq = HashSet::new();
//     iter.into_iter().all(move |x| uniq.insert(x))
// }

// // ments
// #[allow(dead_code)]
impl Map {
    pub fn print(&self) {
        // let mut x;
        let mut y = 0;

        print!("   ");
        for i in 0..self.x {
            print!(" {}", i);
        }
        println!("");
        print!("   ");
        for _i in 0..self.x {
            print!("__");
        }
        println!("");

        for pos in 0..(self.x as usize * self.y as usize / size_of::<i8>()) {
            if pos % self.x as usize == 0 {
                print!("\n{} |", y);
                y = y + 1;
            }
            let nb = unsafe { *self.ptr.offset(pos as _) };

            match from_digit(nb as u32, 10) {
                Some(v) => {
                    print!(" {}", v);
                }
                None => {
                    print!(" {}", nb as char);
                }
            }
        }
        println!("\n");
    }
}
