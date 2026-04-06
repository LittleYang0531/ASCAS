<script setup lang="ts">
import "../assets/markdown.css";
import "../assets/hljs.css";
import "../assets/code.css";
import { ref, watchEffect } from "vue";

const props = defineProps<{
    text: string;
}>();
const html = ref("");

import markdownit from "markdown-it";
import markdownItKatex from "@vscode/markdown-it-katex";
import hljs from "highlight.js";

function render(text: string) {
    const md = markdownit({
        html: true,
        breaks: true,
        highlight: function (str: string, lang: string): string {
            if (str[str.length - 1] == '\n') str = str.substr(0, str.length - 1);
            if (lang && hljs.getLanguage(lang)) {
                try {
                    var code: string = '<pre class="hljs"><code><ul>';
                    var strs: string[] = str.split('\n');
                    for (var i = 0; i < strs.length; i++) 
                        code += '<li class="hljs language-' + lang + '">' + hljs.highlight(lang, strs[i]!, true).value + '</li>'
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
    md.use(markdownItKatex);
    return md.render(text);
}
watchEffect(() => {
    html.value = render(props.text);
});
</script>

<template>
    <div v-html="html" class="markdown-text"></div>
</template>