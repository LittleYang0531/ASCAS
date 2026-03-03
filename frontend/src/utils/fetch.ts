export async function myFetch(url: string, options?: RequestInit): Promise<Response> {
    options = options || { credentials: 'include' };
    try {
        const response = await fetch(url, options);
        return response;
    }
    catch (error) {
        throw error;
    }
}