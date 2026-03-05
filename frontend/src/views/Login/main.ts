import { createApp } from 'vue'
import App from './App.vue'

// @mdi/font
import { aliases, mdi } from 'vuetify/iconsets/mdi-svg'
import { mdi_aliases } from '../../mdi_aliases.ts'

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
    .use(createVuetify({ 
        components, 
        icons: { 
            defaultSet: 'mdi',
            aliases: {
                ...aliases,
                ...mdi_aliases
            },
            sets: {
                mdi,
            },
        }, 
    }))
    .mount('#app')
