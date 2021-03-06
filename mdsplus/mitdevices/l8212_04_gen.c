#include <mitdevices_msg.h>
#include <mds_gendevice.h>
#include "l8212_04_gen.h"
int l8212_04__add(struct descriptor *name_d_ptr, struct descriptor *dummy_d_ptr, int *nid_ptr)
{
	static DESCRIPTOR(library_d, "MIT$DEVICES");
	static DESCRIPTOR(model_d, "L8212_04");
	static DESCRIPTOR_CONGLOM(conglom_d, &library_d, &model_d, 0, 0);
	int usage = TreeUSAGE_DEVICE;
	int curr_nid, old_nid, head_nid, status;
	long int flags = NciM_WRITE_ONCE;
	NCI_ITM flag_itm[] = {{2, NciSET_FLAGS, 0, 0},{0, 0, 0, 0}};
	char *name_ptr = strncpy(malloc(name_d_ptr->length+1),name_d_ptr->pointer,name_d_ptr->length);
	flag_itm[0].pointer = (unsigned char *)&flags;
	name_ptr[name_d_ptr->length]=0;
	status = TreeStartConglomerate(L8212_04_K_CONG_NODES);
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
	ADD_NODE(:NAME, TreeUSAGE_TEXT)
	ADD_NODE(:COMMENT, TreeUSAGE_TEXT)
#define expr " 5.0	"
	ADD_NODE_EXPR(:FREQ, TreeUSAGE_NUMERIC)
#undef expr
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:EXT_CLOCK_IN, TreeUSAGE_AXIS)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
#define expr " 0.	"
	ADD_NODE_EXPR(:STOP_TRIG, TreeUSAGE_NUMERIC)
#undef expr
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE_INTEGER(:MEMORIES,  1		, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE_STRING(:HEADER,  "99999943219" , TreeUSAGE_TEXT)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE_INTEGER(:PTS,  0		, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE_INTEGER(:NOC,  4		, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE_INTEGER(:OFFSET,  -2048	, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE_ACTION(:INIT_ACTION, INIT, INIT, 50, 0, 0, CAMAC_SERVER, 0)
	ADD_NODE_ACTION(:STORE_ACTION, STORE, STORE, 50, 0, 0, CAMAC_SERVER, 0)
	ADD_NODE(:INPUT_01, TreeUSAGE_SIGNAL)
	flags |= NciM_WRITE_ONCE;
	flags |= NciM_COMPRESS_ON_PUT;
	flags |= NciM_NO_WRITE_MODEL;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:INPUT_01:STARTIDX, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:INPUT_01:ENDIDX, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:INPUT_02, TreeUSAGE_SIGNAL)
	flags |= NciM_WRITE_ONCE;
	flags |= NciM_COMPRESS_ON_PUT;
	flags |= NciM_NO_WRITE_MODEL;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:INPUT_02:STARTIDX, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:INPUT_02:ENDIDX, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:INPUT_03, TreeUSAGE_SIGNAL)
	flags |= NciM_WRITE_ONCE;
	flags |= NciM_COMPRESS_ON_PUT;
	flags |= NciM_NO_WRITE_MODEL;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:INPUT_03:STARTIDX, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:INPUT_03:ENDIDX, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:INPUT_04, TreeUSAGE_SIGNAL)
	flags |= NciM_WRITE_ONCE;
	flags |= NciM_COMPRESS_ON_PUT;
	flags |= NciM_NO_WRITE_MODEL;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:INPUT_04:STARTIDX, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:INPUT_04:ENDIDX, TreeUSAGE_NUMERIC)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	status = TreeEndConglomerate();
		if (!(status & 1)) return status;
	return(TreeSetDefaultNid(old_nid));
}

int l8212_04__part_name(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr, struct descriptor *out_d)
{
	int element = 0, status;
 	NCI_ITM nci_list[] = {{4, NciCONGLOMERATE_ELT, 0, 0},{0,0,0,0}};
	nci_list[0].pointer = (unsigned char *)&element;
	status = TreeGetNci(*(int *)nid_d_ptr->pointer, nci_list);
		if (!(status & 1)) return status;
	switch(element)  {
		case(L8212_04_N_HEAD + 1) : StrFree1Dx(out_d); break;
		case(L8212_04_N_NAME + 1) : COPY_PART_NAME(:NAME) break;
		case(L8212_04_N_COMMENT + 1) : COPY_PART_NAME(:COMMENT) break;
		case(L8212_04_N_FREQ + 1) : COPY_PART_NAME(:FREQ) break;
		case(L8212_04_N_EXT_CLOCK_IN + 1) : COPY_PART_NAME(:EXT_CLOCK_IN) break;
		case(L8212_04_N_STOP_TRIG + 1) : COPY_PART_NAME(:STOP_TRIG) break;
		case(L8212_04_N_MEMORIES + 1) : COPY_PART_NAME(:MEMORIES) break;
		case(L8212_04_N_HEADER + 1) : COPY_PART_NAME(:HEADER) break;
		case(L8212_04_N_PTS + 1) : COPY_PART_NAME(:PTS) break;
		case(L8212_04_N_NOC + 1) : COPY_PART_NAME(:NOC) break;
		case(L8212_04_N_OFFSET + 1) : COPY_PART_NAME(:OFFSET) break;
		case(L8212_04_N_INIT_ACTION + 1) : COPY_PART_NAME(:INIT_ACTION) break;
		case(L8212_04_N_STORE_ACTION + 1) : COPY_PART_NAME(:STORE_ACTION) break;
		case(L8212_04_N_INPUT_01 + 1) : COPY_PART_NAME(:INPUT_01) break;
		case(L8212_04_N_INPUT_01_STARTIDX + 1) : COPY_PART_NAME(:INPUT_01:STARTIDX) break;
		case(L8212_04_N_INPUT_01_ENDIDX + 1) : COPY_PART_NAME(:INPUT_01:ENDIDX) break;
		case(L8212_04_N_INPUT_02 + 1) : COPY_PART_NAME(:INPUT_02) break;
		case(L8212_04_N_INPUT_02_STARTIDX + 1) : COPY_PART_NAME(:INPUT_02:STARTIDX) break;
		case(L8212_04_N_INPUT_02_ENDIDX + 1) : COPY_PART_NAME(:INPUT_02:ENDIDX) break;
		case(L8212_04_N_INPUT_03 + 1) : COPY_PART_NAME(:INPUT_03) break;
		case(L8212_04_N_INPUT_03_STARTIDX + 1) : COPY_PART_NAME(:INPUT_03:STARTIDX) break;
		case(L8212_04_N_INPUT_03_ENDIDX + 1) : COPY_PART_NAME(:INPUT_03:ENDIDX) break;
		case(L8212_04_N_INPUT_04 + 1) : COPY_PART_NAME(:INPUT_04) break;
		case(L8212_04_N_INPUT_04_STARTIDX + 1) : COPY_PART_NAME(:INPUT_04:STARTIDX) break;
		case(L8212_04_N_INPUT_04_ENDIDX + 1) : COPY_PART_NAME(:INPUT_04:ENDIDX) break;
		default : status = TreeILLEGAL_ITEM;
	}
	return status;
}

	extern int l8212_04___init();
#define free_xd_array { int i; for(i=0; i<1;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}
#define error(nid,code,code1) {free_xd_array return GenDeviceSignal(nid,code,code1);}

int l8212_04__init(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
	declare_variables(InInitStruct)
static struct {
	short code;
	int value;} noc_t[] = {
	0, 1,
	1, 2,
	2, 4,
	0, 0 };
static struct {
	short code;
	int value;} offset_t[] = {
	0, 0,
	1, -2048,
	2, -4096,
	0, 0 };
	struct descriptor_xd work_xd[1];
	int xd_count = 0;
	memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 1);
	initialize_variables(InInitStruct)

	read_string_error(L8212_04_N_NAME,name, DEV$_BAD_NAME);
	read_integer_error(L8212_04_N_NOC, noc, L8212$_BAD_NOC);
	check_integer_set(noc, noc_t, 3, L8212$_BAD_NOC);
	read_integer_error(L8212_04_N_PTS, pts, L8212$_BAD_PTS);
	check_range(pts, 0, 7, L8212$_BAD_PTS);
	read_integer_error(L8212_04_N_OFFSET, offset, L8212$_BAD_OFFSET);
	check_integer_set(offset, offset_t, 3, L8212$_BAD_OFFSET);
	status = l8212_04___init(nid_d_ptr, &in_struct);
	free_xd_array
	return status;
}
#undef free_xd_array


	extern int l8212_04___store();
#define free_xd_array { int i; for(i=0; i<2;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}

int l8212_04__store(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
	declare_variables(InStoreStruct)
static struct {
	short code;
	int value;} offset_t[] = {
	0, 0,
	1, -2048,
	2, -4096,
	0, 0 };
	struct descriptor_xd work_xd[2];
	int xd_count = 0;
	memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 2);
	initialize_variables(InStoreStruct)

	read_string_error(L8212_04_N_NAME,name, DEV$_BAD_NAME);
	read_integer_error(L8212_04_N_MEMORIES, memories, L8212$_BAD_MEMORIES);
	check_range(memories, 1, 16, L8212$_BAD_MEMORIES);
	read_string_error(L8212_04_N_HEADER,header, L8212$_BAD_HEADER);
	read_integer_error(L8212_04_N_OFFSET, offset, L8212$_BAD_OFFSET);
	check_integer_set(offset, offset_t, 3, L8212$_BAD_OFFSET);
	status = l8212_04___store(nid_d_ptr, &in_struct);
	free_xd_array
	return status;
}
#undef free_xd_array


	extern int l8212_04___trigger();
#define free_xd_array { int i; for(i=0; i<1;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}

int l8212_04__trigger(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
	declare_variables(InTriggerStruct)
	struct descriptor_xd work_xd[1];
	int xd_count = 0;
	memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 1);
	initialize_variables(InTriggerStruct)

	read_string_error(L8212_04_N_NAME,name, DEV$_BAD_NAME);
	status = l8212_04___trigger(nid_d_ptr, &in_struct);
	free_xd_array
	return status;
}
#undef free_xd_array
