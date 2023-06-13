// use super::*;

fn collect_pathways(game: &mut Game, data: &Vec<FWData>) -> Vec<FWFullDataPath> {
    data.into_iter()
        .filter_map(|d| {
            FWFullDataPath::new_check_pathway(d.id_msg, d.coord, game.map.astar(d.coord))
        })
        .collect()
}

fn find_min_pathway(data: &Vec<FWFullDataPath>) -> usize {
    let index = data
        .iter()
        .enumerate()
        .min_by(|(_, a), (_, b)| a.pathway.len().cmp(&b.pathway.len()))
        .map(|(index, _)| index)
        .unwrap();

    index
}

pub(super) fn response(game: &mut Game, ipc: &mut IPC, data: FindWayIncRq) {
    // Находим пути для союзников.
    let ally_pathways: Vec<FWFullDataPath> = collect_pathways(game, &data.ally);

    // Находим ближайщего союзника.
    let ally_closest_index = find_min_pathway(&ally_pathways);

    // Находим пути для противников.
    let foe_pathways: Vec<FWFullDataPath> = collect_pathways(game, &data.foe);

    // Находим ближайщего противника.
    let foe_closest_index = find_min_pathway(&foe_pathways);

    // Формируем для отправки.
    let data = FindWayOutRs {
        receiver_id_msg: data.sender_id_msg,
        ally: FWFullData::new(
            ally_pathways[ally_closest_index].id_msg,
            ally_pathways[ally_closest_index].coord,
            ally_pathways[ally_closest_index].pathway.len(),
        ),
        foe: FWFullData::new(
            foe_pathways[foe_closest_index].id_msg,
            foe_pathways[foe_closest_index].coord,
            foe_pathways[foe_closest_index].pathway.len(),
        ),
    };

    game.comm
        .send(
            CommTypeOutgoing::FindWay(FindWayTypeOutgoing::Rs(data)),
            ipc,
            IPC_NOWAIT,
        )
        .unwrap();
}

pub(super) fn request(
    game: &mut Game,
    ipc: &mut IPC,
    receiver_id_msg: i64,
    ally: Vec<FWData>,
    foe: Vec<FWData>,
) -> Result<(FindWayIncRs), BoxError> {
    // Формируем для отправки.
    let data = FindWayOutRq {
        receiver_id_msg: receiver_id_msg,
        ally: ally,
        foe: foe,
    };

    game.comm.send(
        CommTypeOutgoing::FindWay(FindWayTypeOutgoing::Rq(data)),
        ipc,
        IPC_NOWAIT,
    )?;

    let msg = game.comm.recv(ipc, IPC_WAIT);

    if let Err(e) = msg {
        return (Err(e));
    }

    // if let Some(CommTypeIncoming::Position(PosTypeIncoming::Rs(data))) = msg.unwrap() {
    if let Some(CommTypeIncoming::FindWay(FindWayTypeIncoming::Rs(data))) = msg.unwrap() {
        return Ok((data));
        // alive_data.push(data);
    } else {
        return Err(format!("E! Wrong incoming msg type!").into());
    }
}
