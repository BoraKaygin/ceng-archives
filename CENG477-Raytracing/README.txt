If needed, head into the raytracer folder and compile program by using the "make" command in the terminal

(On Windows) Run the raytracer by using:
./raytracer.exe ./inputs/<input_name>.xml

Will produce output image in ppm format.
- if the scene file includes multiple cameras, will produce multiple outputs.
- some scenes will take more time than others due to polygon complexity. Eg: the horse_and_mug.xml scene may take 10-30 minutes depending on the running computer's performance.

Results will be practically identical to the sample outputs given in the outputs folder.