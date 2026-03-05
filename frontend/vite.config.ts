import { createLogger, defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { exec } from 'child_process'

// https://vite.dev/config/
export default defineConfig({
  base: "./",
  plugins: [
    vue(),
  ],
  server: {
		host: true, // same as "--host" flag
	},
  build: {
    rollupOptions: {
      input: {
        index: 'index.html',
        login: 'login.html',
      }
    }
  }
})

createLogger().info("Updating mdi_aliases.ts...", { timestamp: true });
exec("bash update_mdi.sh");