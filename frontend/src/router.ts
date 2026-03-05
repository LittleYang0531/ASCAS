import { createRouter, createWebHistory } from 'vue-router';
import Index from './views/Index.vue';
import NProgress from 'nprogress';
import CropList from './views/Crop/List.vue';
import CropDetails from './views/Crop/Details.vue';
import CropCreate from './views/Crop/Create.vue';

const routes = [
    {
        path: '/',
        name: 'Home',
        component: Index
    }, {
        path: '/index',
        name: 'Index',
        component: Index
    }, {
        path: '/crops/list',
        name: 'CropList',
        component: CropList
    }, {
        path: '/crops/:id',
        name: 'CropDetails',
        component: CropDetails
    }, {
        path: '/crops/create',
        name: 'CropCreate',
        component: CropCreate
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