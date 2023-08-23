use=[
    'start','爱理','安洁',
    '妹','咪呜',
    '前三选项'
]
import json
with open('namemap.json','r',encoding='utf8') as ff:
    mp=json.load(ff)
savemap={}
mpmp={}
for _u in use:
    with open(f'ja/{_u}.txt','r',encoding='utf8') as ff:
        ja=ff.read()
    with open(f'zh/{_u}.txt','r',encoding='utf8') as ff:
        zh=ff.read()
        
    zh=zh.split('=========')
    ja=ja.split('=========')
    for  i in range(min(len(zh),len(ja))):
        jai=ja[i].replace('\n','').replace(' ','')
        zhi=zh[i].replace('\n','').replace(' ','')
        if(len( set('。」』？―！、').intersection(set(jai)))==0):
            if(len(jai)<10):
                if(jai not in mp):
                    print(jai)
            if('／' in jai):
                #print(jai)
                xx=jai.split('／')
                xx='／'.join([mp[_] for _ in xx])
                mpmp[jai]=xx
    print(mpmp)
            