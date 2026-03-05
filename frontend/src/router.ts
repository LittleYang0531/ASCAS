import { createRouter, createWebHistory } from 'vue-router';
import NProgress from 'nprogress';

const routes = [
    {
        path: '/',
        name: 'Home',
        component: () => { import('./views/Index.vue') }
    }, {
        path: '/index',
        name: 'Home',
        component: () => { import('./views/Index.vue') }
    }, {
        path: '/crops/list',
        name: 'CropList',
        component: () => { import('./views/Crops/List.vue') }
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