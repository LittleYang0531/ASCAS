import { createRouter, createWebHistory } from 'vue-router';
import NProgress from 'nprogress';

const routes = [
    {
        path: '/',
        name: 'Home',
        component: () => import('./views/Index.vue')
    }, {
        path: '/index',
        name: 'Index',
        component: () => import('./views/Index.vue')
    }, {
        path: '/crops/list',
        name: 'CropList',
        component: () => import('./views/Crop/List.vue')
    }, {
        path: '/crops/:id',
        name: 'CropDetails',
        component: () => import('./views/Crop/Details.vue')
    }, {
        path: '/crops/create',
        name: 'CropCreate',
        component: () => import('./views/Crop/Create.vue')
    }, {
        path: '/teams/list',
        name: 'TeamList',
        component: () => import('./views/Team/List.vue')
    }, {
        path: '/teams/:id',
        name: 'TeamDetails',
        component: () => import('./views/Team/Details.vue')
    }, {
        path: '/teams/create',
        name: 'TeamCreate',
        component: () => import('./views/Team/Create.vue')
    }, {
        path: '/teams/:id/edit',
        name: 'TeamEdit',
        component: () => import('./views/Team/Edit.vue')
    }, {
        path: '/messages/list',
        name: 'MessageList',
        component: () => import('./views/Message/List.vue')
    }, {
        path: '/invite',
        name: 'Invite',
        component: () => import('./views/Invite.vue')
    }, {
        path: '/users/:id',
        name: 'UserDetails',
        component: () => import('./views/User/Details.vue')
    }, {
        path: '/users/edit',
        name: 'UserEdit',
        component: () => import('./views/User/Edit.vue')
    }, {
        path: '/blogs/list',
        name: 'BlogList',
        component: () => import('./views/Blog/List.vue')
    }, {
        path: '/blogs/create',
        name: 'BlogCreate',
        component: () => import('./views/Blog/Create.vue')
    }, {
        path: '/blogs/:id',
        name: 'BlogDetails',
        component: () => import('./views/Blog/Details.vue')
    }, {
        path: '/blogs/:id/edit',
        name: 'BlogEdit',
        component: () => import('./views/Blog/Edit.vue')
    }
];

const router = createRouter({
    history: createWebHistory(import.meta.env.BASE_URL),
    routes,
});

router.afterEach(() => {
    document.scrollingElement?.scrollTo({ top: 0, behavior: 'smooth' });
    NProgress.done();
});

export function goto(name: string, query: any, params: any = {}) {
    router.push({ name, query, params });
}

export function locate(url: string) {
    router.push(url);
}

export default router;