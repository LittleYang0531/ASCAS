import NProgress from "nprogress";
import { showMsg } from "./message";
import { MessageType } from "../models/message";

export async function myFetch(url: string, options?: RequestInit): Promise<Response> {
    if (options == undefined) options = { credentials: 'include' };
    if (options.credentials == undefined) options.credentials = 'include';
    // if (options.body != null)
    //     options.headers = options.headers || { "Content-Type": "application/json" };
    try {
        const response = await fetch(url, options);
        return response;
    }
    catch (error) {
        throw error;
    }
}

export async function newFetch(url: string, options?: RequestInit, callback = () => {}): Promise<Response> {
    var response;
    try {
        response = await myFetch(url, options);
    }
    catch (error) {
        NProgress.done();
        showMsg(MessageType.Error, "网络错误，请稍后再试");
        callback && callback();
        throw error;
    }
    if (response.status == 500) {
        NProgress.done();
        showMsg(MessageType.Error, "服务器错误，请稍后再试");
        callback && callback();
        throw new Error("500 Internal Server Error");
    }
    return response;
}