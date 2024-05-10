function CodeDefine() { 
this.def = new Array();
this.def["rtDW"] = {file: "led_blink_c.html",line:36,type:"var"};
this.def["rtM_"] = {file: "led_blink_c.html",line:39,type:"var"};
this.def["rtM"] = {file: "led_blink_c.html",line:40,type:"var"};
this.def["led_blink_step"] = {file: "led_blink_c.html",line:43,type:"fcn"};
this.def["led_blink_initialize"] = {file: "led_blink_c.html",line:65,type:"fcn"};
this.def["RT_MODEL"] = {file: "led_blink_h.html",line:54,type:"type"};
this.def["DW"] = {file: "led_blink_h.html",line:60,type:"type"};
this.def["int8_T"] = {file: "rtwtypes_h.html",line:64,type:"type"};
this.def["uint8_T"] = {file: "rtwtypes_h.html",line:65,type:"type"};
this.def["int16_T"] = {file: "rtwtypes_h.html",line:66,type:"type"};
this.def["uint16_T"] = {file: "rtwtypes_h.html",line:67,type:"type"};
this.def["int32_T"] = {file: "rtwtypes_h.html",line:68,type:"type"};
this.def["uint32_T"] = {file: "rtwtypes_h.html",line:69,type:"type"};
this.def["real32_T"] = {file: "rtwtypes_h.html",line:70,type:"type"};
this.def["real64_T"] = {file: "rtwtypes_h.html",line:71,type:"type"};
this.def["real_T"] = {file: "rtwtypes_h.html",line:77,type:"type"};
this.def["time_T"] = {file: "rtwtypes_h.html",line:78,type:"type"};
this.def["boolean_T"] = {file: "rtwtypes_h.html",line:79,type:"type"};
this.def["int_T"] = {file: "rtwtypes_h.html",line:80,type:"type"};
this.def["uint_T"] = {file: "rtwtypes_h.html",line:81,type:"type"};
this.def["ulong_T"] = {file: "rtwtypes_h.html",line:82,type:"type"};
this.def["char_T"] = {file: "rtwtypes_h.html",line:83,type:"type"};
this.def["uchar_T"] = {file: "rtwtypes_h.html",line:84,type:"type"};
this.def["byte_T"] = {file: "rtwtypes_h.html",line:85,type:"type"};
this.def["pointer_T"] = {file: "rtwtypes_h.html",line:103,type:"type"};
this.def["main.c:OverrunFlags"] = {file: "main_c.html",line:40,type:"var"};
this.def["main.c:autoReloadTimerLoopVal_S"] = {file: "main_c.html",line:43,type:"var"};
this.def["remainAutoReloadTimerLoopVal_S"] = {file: "main_c.html",line:46,type:"var"};
this.def["main"] = {file: "main_c.html",line:54,type:"fcn"};
this.def["SysTick_Handler"] = {file: "stm32xxxx_it_c.html",line:36,type:"fcn"};
}
CodeDefine.instance = new CodeDefine();
var testHarnessInfo = {OwnerFileName: "", HarnessOwner: "", HarnessName: "", IsTestHarness: "0"};
var relPathToBuildDir = "../ert_main.c";
var fileSep = "\\";
var isPC = true;
function Html2SrcLink() {
	this.html2SrcPath = new Array;
	this.html2Root = new Array;
	this.html2SrcPath["led_blink_c.html"] = "../led_blink.c";
	this.html2Root["led_blink_c.html"] = "led_blink_c.html";
	this.html2SrcPath["led_blink_h.html"] = "../led_blink.h";
	this.html2Root["led_blink_h.html"] = "led_blink_h.html";
	this.html2SrcPath["rtwtypes_h.html"] = "../rtwtypes.h";
	this.html2Root["rtwtypes_h.html"] = "rtwtypes_h.html";
	this.html2SrcPath["STM32_Config_h.html"] = "../STM32_Config.h";
	this.html2Root["STM32_Config_h.html"] = "STM32_Config_h.html";
	this.html2SrcPath["led_blink_External_Functions_h.html"] = "../led_blink_External_Functions.h";
	this.html2Root["led_blink_External_Functions_h.html"] = "led_blink_External_Functions_h.html";
	this.html2SrcPath["main_c.html"] = "../main.c";
	this.html2Root["main_c.html"] = "main_c.html";
	this.html2SrcPath["stm32xxxx_it_c.html"] = "../stm32xxxx_it.c";
	this.html2Root["stm32xxxx_it_c.html"] = "stm32xxxx_it_c.html";
	this.getLink2Src = function (htmlFileName) {
		 if (this.html2SrcPath[htmlFileName])
			 return this.html2SrcPath[htmlFileName];
		 else
			 return null;
	}
	this.getLinkFromRoot = function (htmlFileName) {
		 if (this.html2Root[htmlFileName])
			 return this.html2Root[htmlFileName];
		 else
			 return null;
	}
}
Html2SrcLink.instance = new Html2SrcLink();
var fileList = [
"led_blink_c.html","led_blink_h.html","rtwtypes_h.html","STM32_Config_h.html","led_blink_External_Functions_h.html","main_c.html","stm32xxxx_it_c.html"];
