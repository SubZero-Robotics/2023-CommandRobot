# pip install python-ffmpeg
import ffmpeg
inputStreamUrl = f'http://10.56.90.4:8080/stream'
(
    ffmpeg
    .input(inputStreamUrl)
    .filter('fps', fps=10, round='up')
    .output('%s-%%04d.jpg' % (inputStreamUrl[:-4]), **{'qscale:v': 3})
    .run()
)
