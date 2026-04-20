## 一键安装脚本

```bash
curl https://raw.githubusercontent.com/LittleYang0531/ASCAS/refs/heads/main/autoupdate.sh | bash
```

---

## 示例 Nginx 配置

```
server {
	listen 8000 default_server;
	listen [::]:8000 default_server;

	root /etc/ascas/frontend/dist;

	index index.html index.htm index.nginx-debian.html index.php;

	server_name _;

	location /assets/ {
		try_files $uri $uri/ =404;
	}
	location /login {
		try_files /login.html =404;
	}
	location /index.html {
		try_files $uri $uri/ =404;
	}
	location ~* \.(jpg|jpeg|png|gif|ico|svg|webp)$ {
		try_files $uri $uri/ =404;
	}
	location / {
		rewrite ^(.*) /index.html;
	}
}
```

```
server {
	include "ua_deny.conf";
	listen 443 ssl;
	listen [::]:443 ssl;
	server_name ascas.littleyang.com.cn;
	ssl_certificate /etc/letsencrypt/live/littleyang.com.cn/fullchain.pem; # managed by Certbot
	ssl_certificate_key /etc/letsencrypt/live/littleyang.com.cn/privkey.pem; # managed by Certbot
	location /api/ {
		rewrite /api/(.*) /$1 break;
		proxy_pass http://localhost:8003;
		proxy_set_header   Upgrade $http_upgrade;
		proxy_set_header   Connection "upgrade";
		proxy_set_header Host $proxy_host;
		proxy_set_header X-Real-IP $remote_addr;
		proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
		proxy_connect_timeout   1h;
		proxy_send_timeout      1h;
		proxy_read_timeout      1h;
	}
	location / {
		proxy_pass http://localhost:8000;
		proxy_set_header Host $proxy_host;
		proxy_set_header X-Real-IP $remote_addr;
		proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
	}
}

# 强制 https
server {
	include "ua_deny.conf";
    listen 80;
    server_name localhost;
    rewrite ^(.*)$ https://$host$1 permanent;
}
```