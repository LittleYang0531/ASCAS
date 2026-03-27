export const API_BASE_URL = import.meta.env.PROD ? 'https://ascas.littleyang.com.cn/api' : 'http://localhost:8080';
export const WS_BASE_URL = import.meta.env.PROD ? 'wss://ascas.littleyang.com.cn/api' : 'ws://localhost:8080';
export const WEB_NAME = 'ASCAS';
export const imageMaxSize = 512;

export const login_background_urls = [
    `${import.meta.env.BASE_URL}bg1.jpg`,
    `${import.meta.env.BASE_URL}bg2.jpg`,
];

// const defaultConfig = {
//     apiBase: API_BASE_URL
// };

// export function getConfig() {
    
// }