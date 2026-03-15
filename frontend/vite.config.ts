import { createLogger, defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import run from 'vite-plugin-run'
import { exec } from 'child_process';

// https://vite.dev/config/
export default defineConfig({
  plugins: [
    vue(),
    run([
      {
        name: "update_mdi",
        run: [ "true" ],
        condition: (file) => {
          if (file.endsWith("mdi_aliases.ts")) return true;
          createLogger().info("Updating mdi_aliases.ts...", { timestamp: true });
          exec("bash update_mdi.sh");
          return true;
        },
      }
    ])
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