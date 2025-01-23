extern	unsigned short Init_cmd();		/* INIT Command */
extern	unsigned short Media_check_cmd();	/* Media Check Command */
extern  unsigned short Build_bpb_cmd();       /* Build BPB Command */
extern	unsigned short Ioctl_input_cmd();	/* IOCTL Input Command */
extern	unsigned short Input_cmd();		/* Input Command */
extern	unsigned short Input_no_wait_cmd();	/* Input No Wait Command */
extern	unsigned short Input_status_cmd();	/* Input Status Command */
extern	unsigned short Input_flush_cmd();	/* Input Flush Command */
extern	unsigned short Output_cmd();		/* Output Command */
extern	unsigned short Output_verify_cmd();	/* Output Verify Command */
extern	unsigned short Output_status_cmd();	/* Output Status Command */
extern	unsigned short Output_flush_cmd();	/* Output Flush Command */
extern	unsigned short Ioctl_output_cmd();	/* IOCTL Output Command */
extern	unsigned short Dev_open_cmd();	/* Device Open Command */
extern  unsigned short Dev_close_cmd();	/* Device Close Command */
extern	unsigned short Remove_media_cmd();	/* Remove media command */
extern	unsigned short Ioctl_cmd();		/* GENERIC IOCTL Command */
extern	unsigned short Get_l_d_map_cmd();	/* GET Logical Device Map */
extern	unsigned short Set_l_d_map_cmd();	/* SET Logical Device Map */
extern  unsigned short Unknown_cmd();		/* UNKNOWN Command Default */
