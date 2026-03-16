export function isJSON(str: string): boolean {
    try {
        const obj = JSON.parse(str);
        return typeof obj === 'object' && obj !== null;
    } catch (e) {
        return false;
    }
}