import { createRouter, createWebHistory } from 'vue-router';
import Index from './views/Index.vue';
import NProgress from 'nprogress';
import CropList from './views/Crop/List.vue';
import CropDetails from './views/Crop/Details.vue';
import CropCreate from './views/Crop/Create.vue';
import TeamList from './views/Team/List.vue';
import TeamDetails from './views/Team/Details.vue';
import TeamCreate from './views/Team/Create.vue';
import TeamEdit from './views/Team/Edit.vue';
import MessageList from './views/Message/List.vue';
import Invite from './views/Invite.vue';

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
    }, {
        path: '/teams/list',
        name: 'TeamList',
        component: TeamList
    }, {
        path: '/teams/:id',
        name: 'TeamDetails',
        component: TeamDetails
    }, {
        path: '/teams/create',
        name: 'TeamCreate',
        component: TeamCreate
    }, {
        path: '/teams/:id/edit',
        name: 'TeamEdit',
        component: TeamEdit
    }, {
        path: '/messages/list',
        name: 'MessageList',
        component: MessageList
    }, {
        path: '/invite',
        name: "Invite",
        component: Invite
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