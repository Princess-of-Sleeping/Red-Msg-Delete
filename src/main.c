#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/processmgr.h>

#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>

#include <taihen.h>


int vshSblQafMgrIsAllowMarlinTest(void);


static void log_write(const char *path, const char *buffer, size_t length){

	SceUID fd = sceIoOpen(path,
		SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 0777);
	if (fd < 0)
		return;

	sceIoWrite(fd, buffer, length);
	sceIoClose(fd);
}


static SceUID hooks[10];
static uint8_t current_hook = 0;


void InjectDataFunction(modid, addr, buf, size){

	hooks[current_hook] = taiInjectData(modid, 0, addr, buf, size);

	current_hook++;
}





void _start() __attribute__ ((weak, alias("module_start")));
int module_start(SceSize args, void *argp) {
	tai_module_info_t info;
	info.size = sizeof(info);



	if (taiGetModuleInfo("SceShell", &info) >= 0) {


		/* "manufacturing mode" / len : 35 */

		int addr0 = -1;

		/* "This testing kit is not activated." / len : 67 */

		int addr1 = -1;


		/* "This development kit is not activated." / len : 75 */

		int addr2 = -1;


		/* "The backup battery has failed." / len : 59 */

		int addr3 = -1;


		/* "Cannot check expiration date. Please set date via Internet." / len : 117 */

		int addr4 = -1;


		/* "This Testing Kit is expired. See DevKit/TestKit Activation User's Guide." / len : 143 */

		int addr5 = -1;


		/* "This Development Kit is expired. See DevKit/TestKit Activation User's Guide." / len : 151 */

		int addr6 = -1;


		/* "This testing kit expires in %2d day +%02d:%02d:%02d" / len : 101 */

		int addr7 = -1;


		/* "This development kit expires in %2d day +%02d:%02d:%02d" / len : 109 */

		int addr8 = -1;


		switch (info.module_nid) {


			case 0x0552F692: { // retail 3.60 SceShell

				addr0 = 0x50D62C;
				addr1 = 0x50D654;
				addr2 = 0x50D69C;
				addr3 = 0x50D6EC;
				addr4 = 0x50D72C;
				addr5 = 0x50D7A4;
				addr6 = 0x50D838;
				addr7 = 0x50D8D4;
				addr8 = 0x50D9A9;

				break;

			}

			case 0xEAB89D5C: // PTEL 3.60 SceShell
			{

				addr0 = 0x501898;
				addr1 = 0x5018C0;
				addr2 = 0x501908;
				addr3 = 0x501958;
				addr4 = 0x501998;
				addr5 = 0x501A10;
				addr6 = 0x501AA4;
				addr7 = 0x501B40;
				addr8 = 0x501BA8;

				break;

			}


			case 0x6CB01295: // PDEL 3.60 SceShell
			{

				addr0 = 0x4FC6B8;
				addr1 = 0x4FC6E0;
				addr2 = 0x4FC728;
				addr3 = 0x4FC778;
				addr4 = 0x4FC7B8;
				addr5 = 0x4FC830;
				addr6 = 0x4FC8C4;
				addr7 = 0x4FC960;
				addr8 = 0x4FC9C8;

				break;
			}


			default: {

				//log_write("ux0:red_msg_delete_log.txt", &info, 128);

			}

		}




			if(addr0 >= 0)InjectDataFunction(info.modid, addr0, "\0", 35);
			if(addr1 >= 0)InjectDataFunction(info.modid, addr1, "\0", 67);
			if(addr2 >= 0)InjectDataFunction(info.modid, addr2, "\0", 75);
			if(addr3 >= 0)InjectDataFunction(info.modid, addr3, "\0", 59);
			if(addr4 >= 0)InjectDataFunction(info.modid, addr4, "\0", 117);
			if(addr5 >= 0)InjectDataFunction(info.modid, addr5, "\0", 143);
			if(addr6 >= 0)InjectDataFunction(info.modid, addr6, "\0", 151);
			if(addr7 >= 0)InjectDataFunction(info.modid, addr7, "\0", 101);
			if(addr8 >= 0)InjectDataFunction(info.modid, addr8, "\0", 109);


	}

	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp) {


	while (current_hook-- > 0){
		taiInjectRelease(hooks[current_hook]);
	}



	return SCE_KERNEL_STOP_SUCCESS;
}