import { createRouter, createWebHistory } from 'vue-router';
import Index from './views/Index.vue';
import NProgress from 'nprogress';

const routes = [
    {
        path: '/',
        name: 'Home',
        component: Index
    }
];

const router = createRouter({
    history: createWebHistory(import.meta.env.BASE_URL),
    routes,
});

router.afterEach(() => {
    NProgress.done();
});

export function goto(name: string, query: any, params: any = {}) {
    router.push({ name, query, params });
}

export function locate(url: string) {
    router.push(url);
}

export default router;