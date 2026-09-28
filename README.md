# IT Labs: Westside Family Clinic

Interactive IT help desk and SOC practice labs that run entirely in the browser.

Open `index.html` (or the GitHub Pages site) and pick a ticket. Each lab gives you a simulated Linux terminal with its own users, files, services and logs. Nothing touches a real system.

## Labs

| # | Ticket | Skills |
|---|--------|--------|
| 1 | Front desk user locked out | faillock, chage, auth.log |
| 2 | New hire onboarding | useradd, usermod, chmod, passwd |
| 3 | Permission denied on shared folder | chown -R, chmod 2770, groups |
| 4 | Disk full on intranet server | df, du, find, safe cleanup |
| 5 | Web service will not start | systemctl, journalctl, nginx -t |
| 6 | EHR and lab portal unreachable | ping, nslookup, curl, /etc/hosts, resolv.conf |
| 7 | SOC: SSH brute-force alert | grep, sort, uniq on auth.log |
| 8 | SOC: suspicious process and persistence | ps, ss, kill, crontab |

Each lab has Briefing, progressive Hints, a Check button that says exactly what is still wrong, and Reset. Progress is saved in your browser (localStorage). Completing a lab unlocks an honest resume line and a short "what you just learned" summary.

## Tech

One self-contained HTML file with inline CSS and JavaScript. No dependencies, works offline and on phones.

Westside Family Clinic and everyone in it are fictional.
