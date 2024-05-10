function RTW_rtwnameSIDMap() {
	this.rtwnameHashMap = new Array();
	this.sidHashMap = new Array();
	this.rtwnameHashMap["<Root>"] = {sid: "led_blink"};
	this.sidHashMap["led_blink"] = {rtwname: "<Root>"};
	this.rtwnameHashMap["<Root>/GPIO_Write"] = {sid: "led_blink:2"};
	this.sidHashMap["led_blink:2"] = {rtwname: "<Root>/GPIO_Write"};
	this.rtwnameHashMap["<Root>/Pulse Generator"] = {sid: "led_blink:5"};
	this.sidHashMap["led_blink:5"] = {rtwname: "<Root>/Pulse Generator"};
	this.rtwnameHashMap["<Root>/STM32_Config"] = {sid: "led_blink:1"};
	this.sidHashMap["led_blink:1"] = {rtwname: "<Root>/STM32_Config"};
	this.getSID = function(rtwname) { return this.rtwnameHashMap[rtwname];}
	this.getRtwname = function(sid) { return this.sidHashMap[sid];}
}
RTW_rtwnameSIDMap.instance = new RTW_rtwnameSIDMap();
