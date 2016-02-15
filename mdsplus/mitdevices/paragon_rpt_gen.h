#define PARAGON_RPT_K_CONG_NODES 7
#define PARAGON_RPT_N_HEAD 0
#define PARAGON_RPT_N_COMMENT 1
#define PARAGON_RPT_N_REPORT_NAME 2
#define PARAGON_RPT_N_TEXT 3
#define PARAGON_RPT_N_ANALOG 4
#define PARAGON_RPT_N_DIGITAL 5
#define PARAGON_RPT_N_STORE_ACTION 6
typedef struct {
	struct descriptor_xd *__xds;
	int __num_xds;
	int head_nid;
	char *report_name;
} InStoreStruct;
