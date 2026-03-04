import { createApp } from 'vue'
import App from './App.vue'

// @mdi/font
import '@mdi/font/css/materialdesignicons.css'

// nprogress
import NProgress from 'nprogress'
import 'nprogress/nprogress.css'
NProgress.configure({
    parent: ".nProcessBar"
})

// vuetify
import 'vuetify/styles/main.css'
import { createVuetify } from 'vuetify'
import * as components from 'vuetify/components'

createApp(App)
    .use(createVuetify({ components }))
    .mount('#app')
