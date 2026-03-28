<script setup lang="ts">
import "../assets/markdown.css";
import "../assets/hljs.css";
import "../assets/code.css";
import { ref, watchEffect } from "vue";

const props = defineProps<{
    text: string;
}>();
const html = ref("");

async function render(text: string) {
    const markdownit = await import('markdown-it');
    const markdownItKatex = await import('@vscode/markdown-it-katex');
    const hljs = await import('highlight.js');
    const md = markdownit.default({
        html: true,
        breaks: true,
        highlight: function (str: string, lang: string): string {
            if (str[str.length - 1] == '\n') str = str.substr(0, str.length - 1);
            if (lang && hljs.default.getLanguage(lang)) {
                try {
                    var code: string = '<pre class="hljs"><code><ul>';
                    var strs: string[] = str.split('\n');
                    for (var i = 0; i < strs.length; i++) 
                        code += '<li class="hljs language-' + lang + '">' + hljs.default.highlight(lang, strs[i]!, true).value + '</li>'
                    code += '</ul></code></pre>';
                    return code;
                } catch (__) {}
            }
            var code: string = '<pre class="hljs"><code><ul>';
            var strs: string[] = str.split('\n');
            for (var i = 0; i < strs.length; i++) 
                code += '<li class="hljs language-' + lang + '">' + md.utils.escapeHtml(strs[i]!) + '</li>'
            code += '</ul></code></pre>';
            return code;
        }
    });
    md.use(markdownItKatex.default);
    return md.render(text);
}
watchEffect(async () => {
    html.value = await render(props.text);
});
</script>

<template>
    <div v-html="html" class="markdown-text"></div>
</template>