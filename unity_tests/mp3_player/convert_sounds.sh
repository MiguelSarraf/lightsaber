SOUNDS_PATH=../../sounds/
OUTPUT_PATH=/media/user/sd_card_name

sudo apt-get install ffmpeg
mkdir $OUTPUT_PATH/MP3

cp $SOUNDS_PATH/power_up.mp3 $OUTPUT_PATH/MP3/0001.mp3
cp $SOUNDS_PATH/shut_down.mp3 $OUTPUT_PATH/MP3/0002.mp3
cp $SOUNDS_PATH/humming.mp3 $OUTPUT_PATH/MP3/0003.mp3

echo Copied default files to $OUTPUT_PATH/MP3/

cont=4
for file in $(find $SOUNDS_PATH -name 'swing*.mp3') ; do
	echo Copying $file to $OUTPUT_PATH/MP3/$(printf %04d $cont).mp3
	cp $file $OUTPUT_PATH/MP3/$(printf %04d $cont).mp3
	cont=$(($cont+1))
done
