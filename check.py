use=[
    'start','爱理','安洁',
    '妹','咪呜',
    '前三选项'
]
import json
with open('namemap.json','r',encoding='utf8') as ff:
    mp=json.load(ff)
for check in use:
    with open(f'zh/{check}.txt','r',encoding='utf8') as ff:
        zh=ff.read()
    with open(f'ja/{check}.txt','r',encoding='utf8') as ff:
        ja=ff.read()

    zh=zh.split('=========')
    ja=ja.split('=========')
    
    print(len(zh),len(ja))
    for  i in range(min(len(zh),len(ja))):
        jai=ja[i].replace('\n','').replace(' ','')
        zhi=zh[i].replace('\n','').replace(' ','')
        if(jai in mp):
            if(mp[jai]!=zhi):
                print(mp[jai].encode('utf8'),zhi.encode('utf8'))
                print(i,zh[i],zh[i+1],ja[i],ja[i+1])
                break