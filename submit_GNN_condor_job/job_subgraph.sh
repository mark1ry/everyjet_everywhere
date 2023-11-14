#!/bin/bash

export EOS_MGM_URL=root://eosuser.cern.ch

# >>> conda initialize >>>
# !! Contents within this block are managed by 'conda init' !!
__conda_setup="$('/eos/user/m/moriolpe/anaconda3/bin/conda' 'shell.bash' 'hook' 2> /dev/null)"
if [ $? -eq 0 ]; then
    eval "$__conda_setup"
else
    if [ -f "/usr/etc/profile.d/conda.sh" ]; then
        . "/usr/etc/profile.d/conda.sh"
    else
        export PATH="/usr/bin:$PATH"
    fi
fi
unset __conda_setup
# <<< conda initialize <<<
conda init
conda activate subgraph
echo $CONDA_PREFIX

cd subgraph-contraction-ttbar

# Copy input datasets
mkdir data
eos cp /eos/user/m/moriolpe/mphys_project/matched_calib_ttbar.root ./data/
eos cp /eos/user/m/moriolpe/mphys_project/ttbar_allhad.json ./data/
mkdir ./data/output2


python BuildGraphs.py -f ./data/matched_calib_ttbar.root -c ./data/ttbar_allhad.json -o ./data/output2
#python Train.py -c ./presets/ttbar_allhad.json

# Copy output model
eos cp ./data/*.* /eos/user/m/moriolpe/mphys_project/.
eos cp -r ./data/output2 /eos/user/m/moriolpe/mphys_project/.

cd ..
