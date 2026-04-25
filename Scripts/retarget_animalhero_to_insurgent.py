import unreal


OUT_DIR = "/Game/RPGHeroSquad/RetargetedToInsurgent"
RIG_DIR = "/Game/RPGHeroSquad/RetargetedToInsurgent/Rigs"

SOURCE_MESH_PATH = "/Game/RPGHeroSquad/Mesh/Character/SK_DogPBR.SK_DogPBR"
TARGET_MESH_PATH = "/Game/Modern_Insurgent_7/Mesh/SK_Insurgent_P7.SK_Insurgent_P7"

ANIM_PATHS = [
    "/Game/RPGHeroSquad/Animation/AnimalHero/Anim_Attack01_AnimalHero.Anim_Attack01_AnimalHero",
    "/Game/RPGHeroSquad/Animation/AnimalHero/Anim_Attack02_AnimalHero.Anim_Attack02_AnimalHero",
    "/Game/RPGHeroSquad/Animation/AnimalHero/Anim_Defend_AnimalHero.Anim_Defend_AnimalHero",
    "/Game/RPGHeroSquad/Animation/AnimalHero/Anim_DieA_AnimalHero.Anim_DieA_AnimalHero",
    "/Game/RPGHeroSquad/Animation/AnimalHero/Anim_DieRecover_AnimalHero.Anim_DieRecover_AnimalHero",
    "/Game/RPGHeroSquad/Animation/AnimalHero/Anim_Dizzy_AnimalHero.Anim_Dizzy_AnimalHero",
    "/Game/RPGHeroSquad/Animation/AnimalHero/Anim_GetHit_AnimalHero.Anim_GetHit_AnimalHero",
    "/Game/RPGHeroSquad/Animation/AnimalHero/Anim_Idle_Battle_AnimalHero.Anim_Idle_Battle_AnimalHero",
    "/Game/RPGHeroSquad/Animation/AnimalHero/InPlace/Anim_RunForwardBattle_IP_AnimalHero.Anim_RunForwardBattle_IP_AnimalHero",
    "/Game/RPGHeroSquad/Animation/AnimalHero/InPlace/Anim_SprintForwardBattle_IP_AnimalHero.Anim_SprintForwardBattle_IP_AnimalHero",
    "/Game/RPGHeroSquad/Animation/AnimalHero/InPlace/Anim_WalkForwardBattle_IP_AnimalHero.Anim_WalkForwardBattle_IP_AnimalHero",
]


def load(path):
    obj = unreal.EditorAssetLibrary.load_asset(path)
    if not obj:
        raise RuntimeError("Asset not found: {}".format(path))
    return obj


def ensure_dir(path):
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def create_or_load_ik_rig(asset_name, mesh):
    asset_path = "{}/{}".format(RIG_DIR, asset_name)
    existing = unreal.EditorAssetLibrary.load_asset(asset_path)
    if existing:
        rig = existing
    else:
        rig = unreal.IKRigDefinitionFactory.create_new_ik_rig_asset(RIG_DIR, asset_name)
        if not rig:
            raise RuntimeError("Could not create IK Rig: {}".format(asset_path))

    controller = unreal.IKRigController.get_controller(rig)
    if not controller.set_skeletal_mesh(mesh):
        raise RuntimeError("Could not set skeletal mesh {} on {}".format(mesh.get_name(), asset_name))

    auto_ok = controller.apply_auto_generated_retarget_definition()
    unreal.log("{} auto retarget definition: {}".format(asset_name, auto_ok))

    unreal.EditorAssetLibrary.save_loaded_asset(rig)
    return rig, controller


def add_chain_if_possible(controller, chain_name, start, end):
    try:
        controller.add_retarget_chain(chain_name, start, end, unreal.Name(""))
    except Exception as exc:
        unreal.log_warning("Could not add chain {} ({} -> {}): {}".format(chain_name, start, end, exc))


def fallback_humanoid_chains(controller):
    # These names exist on AnimalHero and many Mixamo/insurgent-style skeletons.
    controller.set_retarget_root("pelvis")
    for chain_name, start, end in [
        ("spine", "spine_01", "head"),
        ("neck", "neck_01", "head"),
        ("head", "head", "head"),
        ("left_arm", "upperarm_l", "hand_l"),
        ("right_arm", "upperarm_r", "hand_r"),
        ("left_leg", "thigh_l", "ball_l"),
        ("right_leg", "thigh_r", "ball_r"),
        ("left_foot", "foot_l", "ball_l"),
        ("right_foot", "foot_r", "ball_r"),
    ]:
        add_chain_if_possible(controller, chain_name, start, end)


def create_or_load_retargeter(source_rig, target_rig, target_mesh):
    asset_name = "RTG_Dog_to_Insurgent"
    asset_path = "{}/{}".format(RIG_DIR, asset_name)
    retargeter = unreal.EditorAssetLibrary.load_asset(asset_path)

    if not retargeter:
        tools = unreal.AssetToolsHelpers.get_asset_tools()
        factory = unreal.IKRetargetFactory()
        retargeter = tools.create_asset(asset_name, RIG_DIR, unreal.IKRetargeter, factory)
        if not retargeter:
            raise RuntimeError("Could not create IK Retargeter: {}".format(asset_path))

    controller = unreal.IKRetargeterController.get_controller(retargeter)
    controller.set_ik_rig(unreal.RetargetSourceOrTarget.SOURCE, source_rig)
    controller.set_ik_rig(unreal.RetargetSourceOrTarget.TARGET, target_rig)
    controller.set_preview_mesh(unreal.RetargetSourceOrTarget.TARGET, target_mesh)

    try:
        controller.add_default_ops()
    except Exception as exc:
        unreal.log_warning("add_default_ops skipped: {}".format(exc))

    try:
        controller.auto_map_chains(unreal.AutoMapChainType.FUZZY, True)
    except Exception as exc:
        unreal.log_warning("fuzzy auto_map_chains failed: {}".format(exc))
        try:
            controller.auto_map_chains(unreal.AutoMapChainType.EXACT, True)
        except Exception as exc2:
            unreal.log_warning("exact auto_map_chains failed: {}".format(exc2))

    unreal.EditorAssetLibrary.save_loaded_asset(retargeter)
    return retargeter


def main():
    ensure_dir(OUT_DIR)
    ensure_dir(RIG_DIR)

    source_mesh = load(SOURCE_MESH_PATH)
    target_mesh = load(TARGET_MESH_PATH)
    anims = [load(path) for path in ANIM_PATHS]

    source_rig, source_controller = create_or_load_ik_rig("IKR_DogPBR", source_mesh)
    target_rig, target_controller = create_or_load_ik_rig("IKR_Insurgent", target_mesh)

    if not source_controller.get_retarget_chains():
        unreal.log_warning("Source auto chains are empty; adding fallback humanoid chains.")
        fallback_humanoid_chains(source_controller)
        unreal.EditorAssetLibrary.save_loaded_asset(source_rig)

    if not target_controller.get_retarget_chains():
        unreal.log_warning("Target auto chains are empty; adding fallback humanoid chains.")
        fallback_humanoid_chains(target_controller)
        unreal.EditorAssetLibrary.save_loaded_asset(target_rig)

    retargeter = create_or_load_retargeter(source_rig, target_rig, target_mesh)

    asset_data = []
    for path in ANIM_PATHS:
        object_path = path.split(".")[0]
        data = unreal.EditorAssetLibrary.find_asset_data(object_path)
        if not data or not data.is_valid():
            raise RuntimeError("Could not find AssetData for {}".format(object_path))
        asset_data.append(data)
    created = unreal.IKRetargetBatchOperation.duplicate_and_retarget(
        asset_data,
        source_mesh,
        target_mesh,
        retargeter,
        "",
        "",
        "",
        "_ToInsurgent",
        False,
        True,
    )

    for data in created:
        source_package = str(data.package_name)
        asset_name = source_package.rsplit("/", 1)[-1]
        target_package = "{}/{}".format(OUT_DIR, asset_name)

        if source_package != target_package:
            if unreal.EditorAssetLibrary.does_asset_exist(target_package):
                unreal.EditorAssetLibrary.delete_asset(target_package)
            if not unreal.EditorAssetLibrary.rename_asset(source_package, target_package):
                raise RuntimeError("Could not move {} to {}".format(source_package, target_package))
            source_package = target_package

        unreal.log("Created retargeted asset: {}".format(source_package))

    unreal.EditorAssetLibrary.save_directory(OUT_DIR, only_if_is_dirty=False, recursive=True)
    unreal.log("Retarget done. Output: {}".format(OUT_DIR))


main()
