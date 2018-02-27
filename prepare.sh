create_Dir(){
    mkdir $1
    cd $1
    mkdir Grid-3
    mkdir Grid-4
    mkdir Grid-5
    mkdir Grid-6
    cd ..
}

create_Dir Text-Results
create_Dir Plot-Results
create_Dir Plot-CompEp-Results
create_Dir Text-CompEp-Results
