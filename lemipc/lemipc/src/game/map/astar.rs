use std::collections::HashMap;
use libc::close;
use super::*;

#[derive(Debug, PartialEq, Eq, Hash, Copy, Clone)]
pub struct Astruct {
    coord: isize,
    parent: Option<isize>,
    // Количество энергии, затрачиваемое на передвижение из стартовой клетки в текущую рассматриваемую клетку,
    // следуя найденному пути к этой клетке; + 10/14: 10 - для ортогональных перемещений, 14 - для диагональных.
    g: isize,
    // Количество энергии, затрачиваемое на передвижение от текущей клетки до конечной клетки.
    h: isize,
    // g + h
    f: isize,
}

impl Astruct {
    fn new(coord: isize, parent: Option<isize>) -> Self {
        Astruct {
            coord,
            parent,
            g: 0,
            h: 0,
            f: 0,
        }
    }

    fn recalc(&mut self, new_parent: &Astruct) {
        self.g = new_parent.g;
        self.parent = Some(new_parent.coord);
        self.f = self.g + self.h;
    }
}

pub fn aa(map: &Map, goal: isize) -> Option<Vec<isize>> {
        let mut opened = vec![];
        let mut closed = HashMap::new();

        None
}

impl Map {
    pub fn astar(&self, end_coord: isize) -> Option<Vec<isize>> {
        let mut opened = vec![];
        let mut closed = HashMap::new();

        // Вставляем начальную точку.
        opened.push(Astruct::new(self.coord, None));

        // Цикл
        let result = loop {
            // Если конечная клетка в открытом списке - путь найден.
            if let Some(index) = opened.iter().position(|o| o.coord == end_coord) {
                closed.insert(opened[index].coord, opened[index]);
                break Some(opened[index].coord as isize);
            // Если список пуст, то пути нету, это тупик.
            } else if opened.is_empty() {
                break None;
            }

            // Ищем минимальное значение f из открытого списка.
            let index_of_min = opened
                .iter()
                .enumerate()
                .min_by(|(_, a), (_, b)| a.f.cmp(&b.f))
                .map(|(index, _)| index)
                .unwrap();

            // Клетка для проверки.
            let current = opened[index_of_min];
            // Добавляем в закрытый список.
            closed.insert(current.coord, current);
            // Удаляем из открытого списка.
            opened.retain(|o| o.coord != current.coord);
            // Проверяем соседние клетки текущей клетки.
            self.astar_extra(&mut opened, &mut closed, current, end_coord);
        };

        // Обрабатываем результат.
        self.astar_build_path(result, &mut closed)
    }

    fn astar_extra(
        &self,
        opened: &mut Vec<Astruct>,
        closed: &mut HashMap<isize, Astruct>,
        current: Astruct,
        end_coord: isize,
    ) {
        // Находим для текущей клетки возможные проходимые соседние клетки.
        let possible_cells = self.get_possible_orthogonal_cells(&current, end_coord);

        // Добавляем в открытый список.
        for cell in possible_cells {
            // Не добавляем клетки, которые уже находятся в ЗАКРЫТОМ списке.
            if let Some(_) = closed.get(&cell.coord) {
                continue;
            }

            // Не добавляем клетки, которые уже находятся в ОТКРЫТОМ списке, при этом:
            // - для клеток из открытого списка пересчитываем показатели, если текущий путь выгодней.
            if let Some(index) = opened.iter().position(|o| o.coord == cell.coord) {
                if cell.g < opened[index].g {
                    opened[index].recalc(&cell);
                }
                continue;
            }
            opened.push(cell);
        }
    }

    pub fn astar_build_path(
        &self,
        res: Option<(isize)>,
        closed: &mut HashMap<isize, Astruct>,
    ) -> Option<Vec<isize>> {
        match res {
            Some(end_coord) => {
                let mut tmp_coord = end_coord;
                let mut path: Vec<isize> = vec![];

                while let Some(c) = closed.get(&tmp_coord) {
                    if let Some(n) = c.parent {
                        path.push(c.coord);
                        tmp_coord = n;
                    } else {
                        break;
                    }
                }
                Some(path.into_iter().rev().collect())
            }
            None => None,
        }
    }

    // Ррасстояние манхэттена в одномерном пространстве.
    fn manhattan_distance(&self, begin: isize, end: isize) -> isize {
        let (x1, y1) = self.convert_to_2d(begin);
        let (x2, y2) = self.convert_to_2d(end);

        isize::abs(x2 - x1) + isize::abs(y2 - y1)
    }

    // 1d.
    /// Функция поиска возможных путей от указанной координаты.
    /// возвращает ветор возможных координат для перемещения или None если таковых нету.
    pub fn get_possible_orthogonal_cells(
        &self,
        parent: &Astruct,
        end_coord: isize,
    ) -> Vec<Astruct> {
        // В лучшем случае 4 возможных пути от координаты (по диагонали и вертикали).
        let mut vec = Vec::with_capacity(MOV_ORTHOGONALITY.len());

        for way in MOV_ORTHOGONALITY {
            if let Some(c) = self.get_coord_by_way(parent.coord, way) {
                let is_empty = unsafe { *self.ptr.offset(c) };

                if is_empty == 0 || c == end_coord {
                    // *self.ptr.offset()
                    let g = parent.g + 10; // без диагонали
                    let h = self.manhattan_distance(c, end_coord);
                    vec.push(Astruct {
                        coord: c,
                        parent: Some(parent.coord),
                        g: g,
                        h: h,
                        f: g + h,
                    });
                }
            }
        }
        vec
    }
}

// #[cfg(test)]
// mod tests {
//     use libc::close;
//     use std::cmp::Ordering;
//     use std::collections::HashMap;

//     use super::*;

//     #[test]
//     fn lol() {
//         let mut area: [u8; 25] = [
//             0, 0, 0, b'x', 1, 0, b'x', 0, b'x', 0, 0, b'x', 0, b'x', 0, 0, b'x', 0, b'x', 0, 0, 0,
//             0, 0, 1,
//         ];

//         let map = Map {
//             ptr: &mut area as *mut u8,
//             x: 5,
//             y: 5,
//             coord: 0,
//         };

//         map.print();

//         // Конечная координата.
//         let end_coord = 24;

//         let path = map.astar(end_coord);

//         println!("path: {:#?}", path);
//     }
// }
