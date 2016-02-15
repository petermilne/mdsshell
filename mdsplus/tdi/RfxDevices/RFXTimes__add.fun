public fun RFXTimes__add(in _path, out _nidout)
{
    DevAddStart(_path, 'RFXTimes', 49, _nidout);
    DevAddNode(_path // ':CLOSE_PNSS', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':CLOSE_PNSS T_CLOSE_PNSS');
    DevAddNode(_path // ':CLOSE_PP1', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PP1 T_CLOSE_PP1');
    DevAddNode(_path // ':CLOSE_PP2', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PP2 T_CLOSE_PP2');
   DevAddNode(_path // ':CLOSE_PP3', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PP3 T_CLOSE_PP3');
   DevAddNode(_path // ':CLOSE_PP4', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PP4 T_CLOSE_PP4');
    DevAddNode(_path // ':CLOSE_PPSC1', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PPSC1 T_CLOSE_PPSC1');
    DevAddNode(_path // ':CLOSE_PPSC2', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PPSC2 T_CLOSE_PPSC2');
    DevAddNode(_path // ':CLOSE_PPSC3', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PPSC3 T_CLOSE_PPSC3');
    DevAddNode(_path // ':CLOSE_PPSC4', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PPSC4 T_CLOSE_PPSC4');
    DevAddNode(_path // ':CLOSE_PTCT_1', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PTCT_1 T_CLOSE_PTCT_1');
    DevAddNode(_path // ':CLOSE_PTCT_2', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PTCT_2 T_CLOSE_PTCT_2');
    DevAddNode(_path // ':CLOSE_PTCT_3', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PTCT_3 T_CLOSE_PTCT_3');
    DevAddNode(_path // ':CLOSE_PTCT_4', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PTCT_4 T_CLOSE_PTCT_4');
    DevAddNode(_path // ':CLOSE_PTCT_5', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PTCT_5 T_CLOSE_PTCT_5');
    DevAddNode(_path // ':CLOSE_PTCT_6', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PTCT_6 T_CLOSE_PTCT_6');
    DevAddNode(_path // ':CLOSE_PTCT_7', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PTCT_7 T_CLOSE_PTCT_7');
    DevAddNode(_path // ':CLOSE_PTCT_8', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_PTCT_8 T_CLOSE_PTCT_8');
    DevAddNode(_path // ':CLOSE_TNSI', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_TNSI T_CLOSE_TNSI');
    DevAddNode(_path // ':CLOSE_TNSR', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_TNSR T_CLOSE_TNSR');
    DevAddNode(_path // ':CLOSE_TNST', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':CLOSE_TNST T_CLOSE_TNST');
    DevAddNode(_path // ':INSRT_PC', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':INSRT_PC T_INSRT_PC');
    DevAddNode(_path // ':INSRT_PTCB', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':INSRT_PTCB T_INSRT_PTCB');
    DevAddNode(_path // ':OPEN_PTSO', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':OPEN_PTSO T_OPEN_PTSO');
	DevAddNode(_path // ':PPSC_C', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':PPSC_C T_PPSC_C');
    DevAddNode(_path // ':START_TF', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':START_TF T_START_TF');
    DevAddNode(_path // ':STOP_TF', 'NUMERIC', *, *, _nid);
	tcl('add tag ' // _path // ':STOP_TF T_STOP_TF');
    DevAddNode(_path // ':START_PM', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':START_PM T_START_PM');
    DevAddNode(_path // ':STOP_PM', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':STOP_PM T_STOP_PM');
    DevAddNode(_path // ':START_PV', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':START_PV T_START_PV');
    DevAddNode(_path // ':STOP_PV', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':STOP_PV T_STOP_PV');
    DevAddNode(_path // ':START_PC', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':START_PC T_START_PC');
    DevAddNode(_path // ':STOP_PC', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':STOP_PC T_STOP_PC');
    DevAddNode(_path // ':START_RFX', 'NUMERIC', -5., *, _nid);
	tcl('add tag '//_path //':START_RFX T_START_RFX');

    DevAddNode(_path // ':START_CP_TC', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':START_CP_TC T_START_CHOP_TC');
    DevAddNode(_path // ':STOP_CP_TC', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':STOP_CP_TC T_STOP_CHOP_TC');
    DevAddNode(_path // ':START_INV_TC', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':START_INV_TC T_START_INV_TC');
    DevAddNode(_path // ':STOP_INV_TC', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':STOP_INV_TC T_STOP_INV_TC');
    DevAddNode(_path // ':START_PWM_TC', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':START_PWM_TC T_START_PWM_TC');
    DevAddNode(_path // ':STOP_PWM_TC', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':STOP_PWM_TC T_STOP_PWM_TC');
    DevAddNode(_path // ':CLOSE_IS_TC', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':CLOSE_IS_TC T_CLOSE_IS_TC');
    DevAddNode(_path // ':OPEN_IS_TC', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':OPEN_IS_TC T_OPEN_IS_TC');
    DevAddNode(_path // ':CROWB_ON_TC', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':CROWB_ON_TC T_CROWBAR_ON_TC');

    DevAddNode(_path // ':START_INV_R', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':START_INV_R T_START_INV_TC_REF');
    DevAddNode(_path // ':STOP_INV_R', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':STOP_INV_R T_STOP_INV_TC_REF');
    DevAddNode(_path // ':START_PWM_R', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':START_PWM_R T_START_PWM_TC_REF');
    DevAddNode(_path // ':STOP_PWM_R', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':STOP_PWM_R T_STOP_PWM_TC_REF');

    DevAddNode(_path // ':START_PR', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':START_PR T_START_PR');
    DevAddNode(_path // ':STOP_PR', 'NUMERIC', *, *, _nid);
	tcl('add tag '//_path //':STOP_PR T_STOP_PR');


    DevAddEnd();
}

       

