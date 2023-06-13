use super::IPCInitOption;
use std::env;

// Количетсво возможных аргументов.
//
// C_MIN_ARG_QT:
// - подразумевается, что процесс является инициализатором игры, должен инициализировать System V IPC
// и создать дочерние процессы.
//
// C_MAX_ARG_QT:
// - подразумевается, что System V IPC инициализированы и остается только подключиться к игре.
const C_MIN_ARG_QT: usize = 5;
const C_MAX_ARG_QT: usize = 7;

// Размер карты.
const C_MIN_MAP_SIZE: usize = 5;
const C_MAX_MAP_SIZE: usize = 100;

// Количество игроков в каждой команде.
const C_MIN_PLAYERS_QT: usize = 2;
const C_MAX_PLAYERS_QT: usize = 2;

// Количество команд.
const C_MIN_TEAMS_QT: usize = 2;
const C_MAX_TEAMS_QT: usize = 2;

const DEF_USAGE: &str = r#"
DEF_USAGE:
	./lemipc [num of teams] [num of players] [map size X] [map size Y]
"#;

type ParseRangeTupl = (Option<usize>, Option<usize>);

#[derive(Default, Debug, Clone, Copy)]
pub struct Args {
    pub ipc_mod: IPCInitOption, // to define an initializer process
    pub x: isize,
    pub y: isize,
    pub qt_teams: u8,
    pub qt_players: u8,
    pub id_team: u8,   // уникальное поле вместе с id_player
    pub id_player: u8, // уникальное поле вместе с id_team
}

impl Args {
    /// Parse args and return Gamesettings struct
    ///
    /// This associative function:
    /// checks the number of arguments;
    /// validation of data input according to constants.
    pub fn from_args() -> Result<Args, Box<dyn std::error::Error>> {
        let mut info = Args::default();

        let ac: Vec<String> = env::args().collect();

        match ac.len() {
            C_MIN_ARG_QT..=C_MAX_ARG_QT => match ac.len() {
                C_MIN_ARG_QT => {
                    info.ipc_mod = IPCInitOption::Create;
                    info.id_team = 1;
                    info.id_player = 1;
                }
                C_MAX_ARG_QT => {
                    info.ipc_mod = IPCInitOption::Join;
                    info.id_team = ac[5].parse::<u8>()?;
                    info.id_player = ac[6].parse::<u8>()?;
                }
                _ => {
                    eprintln!("{}", DEF_USAGE);
                    return Err("E! Wrong arguments count".into());
                }
            },
            _ => {
                eprintln!("{}", DEF_USAGE);
                return Err("E! Wrong arguments count".into());
            }
        }
        info.qt_players = parse_range_u(&ac[1], (Some(C_MIN_PLAYERS_QT), Some(C_MAX_PLAYERS_QT)))?
            .try_into()
            .unwrap();
        info.qt_teams = parse_range_u(&ac[2], (Some(C_MIN_TEAMS_QT), Some(C_MAX_TEAMS_QT)))?
            .try_into()
            .unwrap();
        info.x = parse_range_u(&ac[3], (Some(C_MIN_MAP_SIZE), Some(C_MAX_MAP_SIZE)))? as isize;
        info.y = parse_range_u(&ac[4], (Some(C_MIN_MAP_SIZE), Some(C_MAX_MAP_SIZE)))? as isize;

        Ok(info)
    }
}

/// Converts a string to a number and checks if the number is in the valid range.
///
/// The range may be absent or have only an upper or lower bound.
/// In this case, there will be no check_coordinate for the range or it will be only for the specified upper or lower range.
fn parse_range_u(s: &str, range: ParseRangeTupl) -> Result<usize, Box<dyn std::error::Error>> {
    let num = s.trim().parse::<usize>()?;

    match range {
        (Some(min), Some(max)) => {
            if min > max {
                return Err("E! Min of range must be less ot equal then max.".into());
            }
            if num < min || num > max {
                return Err("E! Out of range.".into());
            }
        }
        (Some(min), _) => {
            if num < min {
                return Err("E! Out of range.".into());
            }
        }
        (_, Some(max)) => {
            if num > max {
                return Err("E! Out of range.".into());
            }
        }
        (None, None) => {}
    }
    Ok(num)
}
