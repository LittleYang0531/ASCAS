import { ref } from "vue";
import { MessageType } from "../models/message";

var errorListener = 0;
var errorListener2 = 0;
export var isError = ref(false);
export var type = ref(MessageType.Success);
export var errorText = ref("");
export function showMsg(errorType: MessageType, msg: string) {
	if (errorListener != 0) { clearTimeout(errorListener); }
	if (errorListener2 != 0) { clearTimeout(errorListener2); }
	isError.value = false;
	errorListener = setTimeout(() => {
		isError.value = true;
		type.value = errorType;
		errorText.value = msg;
	}, 500);
	errorListener2 = setTimeout(() => { isError.value = false; }, 5000);
}