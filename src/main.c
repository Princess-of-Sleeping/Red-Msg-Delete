#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>

#include <taihen.h>

SceUID _vshKernelSearchModuleByName(const char *module_name, int *vsh_buf);

int module_get_offset(SceUID modid, int segidx, uint32_t offset, void *stub_out){

	int res = 0;
	SceKernelModuleInfo info;

	if(segidx > 3){
		return -1;
	}

	if(stub_out == NULL){
		return -2;
	}

	res = sceKernelGetModuleInfo(modid, &info);
	if(res < 0){
		return res;
	}

	if(offset > info.segments[segidx].memsz){
		return -3;
	}

	*(uint32_t *)stub_out = (uint32_t)(info.segments[segidx].vaddr + offset);

	return 0;
}

const char patch[] = {0x00, 0x20, 0x70, 0x47};

int red_msg_inject(tai_module_info_t *info){

	SceUID modid = info->modid;

	switch (info->module_nid){
	case 0x0552F692: // SceShell 3.60 retail
		taiInjectData(modid, 0, 0x14F466, patch, 4);
		break;
	case 0xEAB89D5C: // SceShell 3.60 Testkit
		taiInjectData(modid, 0, 0x14789A, patch, 4);
		break;
	case 0x6CB01295: // SceShell 3.60 Devkit
		taiInjectData(modid, 0, 0x146E9E, patch, 4);
		break;
	case 0x5549BF1F: // SceShell 3.65 retail
		taiInjectData(modid, 0, 0x14F4BE, patch, 4);
		break;
	case 0x587F9CED: // SceShell 3.65 Testkit
		taiInjectData(modid, 0, 0x1478F2, patch, 4);
		break;
	case 0xE6A02F2B: // SceShell 3.65 Devkit
		taiInjectData(modid, 0, 0x146EF6, patch, 4);
		break;
	default:
		return -1;
	}

	return 0;
}

int delete_red_msg_without_enso(tai_module_info_t *info){

	void *shell_top_widget, *widget_activate;
	int (* update_string)(void *widget_ptr, void *a2, int a3);

	SceUID modid = info->modid;

	switch (info->module_nid){
	case 0x0552F692: // SceShell 3.60 retail
		module_get_offset(modid, 1, 0x21BA0, &shell_top_widget);
		break;
	case 0xEAB89D5C: // SceShell 3.60 Testkit
		module_get_offset(modid, 1, 0x20918, &shell_top_widget);
		break;
	case 0x6CB01295: // SceShell 3.60 Devkit
		module_get_offset(modid, 1, 0x208F8, &shell_top_widget);
		break;
	case 0x5549BF1F: // SceShell 3.65 retail
		module_get_offset(modid, 1, 0x21BB0, &shell_top_widget);
		break;
	case 0x587F9CED: // SceShell 3.65 Testkit
		module_get_offset(modid, 1, 0x20928, &shell_top_widget);
		break;
	case 0xE6A02F2B: // SceShell 3.65 Devkit
		module_get_offset(modid, 1, 0x20908, &shell_top_widget);
		break;
	default:
		return -1;
	}

	widget_activate = (void *)(*(int *)(*(int *)(shell_top_widget) + 0x1D8 + 8));

	if(widget_activate != NULL){

		update_string = (void *)(*(uint32_t *)(*(int *)(widget_activate) + 0x118));

		int data[2];

		data[0]= (int)&data[1];
		data[1]= 0;

		update_string(widget_activate, data, 0);
	}

	return 0;
}

void _start() __attribute__ ((weak, alias("module_start")));
int module_start(SceSize args, void *argp){

	tai_module_info_t info;
	info.size = sizeof(info);

	if(taiGetModuleInfo("SceShell", &info) < 0)
		return SCE_KERNEL_START_FAILED;

	int buf[2];

	int some_mode = (_vshKernelSearchModuleByName("SceSysStateMgr", buf) < 0) ? 0 : 1;

	if(some_mode == 0){
		delete_red_msg_without_enso(&info);
	}else{
		red_msg_inject(&info);
	}

	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp){
	return SCE_KERNEL_STOP_SUCCESS;
}
