#include <mitdevices_msg.h>
#include <mds_gendevice.h>
#include "mit_pulse_gen.h"
int mit_pulse__add(struct descriptor *name_d_ptr, struct descriptor *dummy_d_ptr, int *nid_ptr)
{
	static DESCRIPTOR(library_d, "MIT$DEVICES");
	static DESCRIPTOR(model_d, "MIT_PULSE");
	static DESCRIPTOR_CONGLOM(conglom_d, &library_d, &model_d, 0, 0);
	int usage = TreeUSAGE_DEVICE;
	int curr_nid, old_nid, head_nid, status;
	long int flags = NciM_WRITE_ONCE;
	NCI_ITM flag_itm[] = {{2, NciSET_FLAGS, 0, 0},{0, 0, 0, 0}};
	char *name_ptr = strncpy(malloc(name_d_ptr->length+1),name_d_ptr->pointer,name_d_ptr->length);
	flag_itm[0].pointer = (unsigned char *)&flags;
	name_ptr[name_d_ptr->length]=0;
	status = TreeStartConglomerate(MIT_PULSE_K_CONG_NODES);
	if (!(status & 1)) return status;
	status = TreeAddNode(name_ptr, &head_nid, usage);
	if (!(status & 1)) return status;
	*nid_ptr = head_nid;
	 status = TreeSetNci(head_nid, flag_itm);
	status = TreePutRecord(head_nid, (struct descriptor *)&conglom_d,0);
	if (!(status & 1)) return status;
	status = TreeGetDefaultNid(&old_nid);
	if (!(status & 1)) return status;
	status = TreeSetDefaultNid(head_nid);
	if (!(status & 1)) return status;
	ADD_NODE(:COMMENT, TreeUSAGE_TEXT)
	ADD_NODE_INTEGER(:TRIGGER_MODE,  0         , TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE_INTEGER(:OUTPUT_MODE,  0         , TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:TRIGGER, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:EVENT, TreeUSAGE_TEXT)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:DURATION, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:PULSE_TIME, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:OUTPUT, TreeUSAGE_NUMERIC)
	flags |= NciM_WRITE_ONCE;
	flags |= NciM_NO_WRITE_MODEL;
	 status = TreeSetNci(curr_nid, flag_itm);
	status = TreeEndConglomerate();
		if (!(status & 1)) return status;
	return(TreeSetDefaultNid(old_nid));
}

int mit_pulse__part_name(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr, struct descriptor *out_d)
{
	int element = 0, status;
 	NCI_ITM nci_list[] = {{4, NciCONGLOMERATE_ELT, 0, 0},{0,0,0,0}};
	nci_list[0].pointer = (unsigned char *)&element;
	status = TreeGetNci(*(int *)nid_d_ptr->pointer, nci_list);
		if (!(status & 1)) return status;
	switch(element)  {
		case(MIT_PULSE_N_HEAD + 1) : StrFree1Dx(out_d); break;
		case(MIT_PULSE_N_COMMENT + 1) : COPY_PART_NAME(:COMMENT) break;
		case(MIT_PULSE_N_TRIGGER_MODE + 1) : COPY_PART_NAME(:TRIGGER_MODE) break;
		case(MIT_PULSE_N_OUTPUT_MODE + 1) : COPY_PART_NAME(:OUTPUT_MODE) break;
		case(MIT_PULSE_N_TRIGGER + 1) : COPY_PART_NAME(:TRIGGER) break;
		case(MIT_PULSE_N_EVENT + 1) : COPY_PART_NAME(:EVENT) break;
		case(MIT_PULSE_N_DURATION + 1) : COPY_PART_NAME(:DURATION) break;
		case(MIT_PULSE_N_PULSE_TIME + 1) : COPY_PART_NAME(:PULSE_TIME) break;
		case(MIT_PULSE_N_OUTPUT + 1) : COPY_PART_NAME(:OUTPUT) break;
		default : status = TreeILLEGAL_ITEM;
	}
	return status;
}

	extern int mit_pulse___get_setup();
#define free_xd_array { int i; for(i=0; i<0;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}
#define error(nid,code,code1) error_code = code1;
int mit_pulse___get_setup(struct descriptor *nid_d_ptr, InGet_setupStruct *in_ptr)
{
	declare_variables(InGet_setupStruct)
	struct descriptor_xd work_xd[1];
	initialize_variables(InGet_setupStruct)

	read_float_error(MIT_PULSE_N_TRIGGER,trigger, MPB$_BADTIME);
	read_float_error(MIT_PULSE_N_PULSE_TIME,pulse_time, MPB$_BADTIME);
	read_float_error(MIT_PULSE_N_DURATION,duration, MPB$_BADTIME);
	read_integer(MIT_PULSE_N_TRIGGER_MODE, trigger_mode);
	read_integer(MIT_PULSE_N_OUTPUT_MODE, output_mode);
	build_results_and_return;
}

#undef free_xd_array
