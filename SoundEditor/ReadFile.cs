using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

//-----------------------------
// By Jishnu Girish
//-----------------------------
namespace SoundEditor
{
    //---------------------------------------------------------
    // Sound Properties holder
    //---------------------------------------------------------
    public class SoundProperties
    {
        public String name;
        public float volume;
        public float pitch;
        public SoundProperties(String _name,float _vol,float _pitch)
        {
            name = _name;
            volume = _vol;
            pitch = _pitch;
        }
    }

    //---------------------------------------------------------
    // ReadFile class
    //---------------------------------------------------------
    public class ReadFile
    {
        private List<SoundProperties> soundList;
        private List<String> names;

        //---------------------------------------------------------
        // Initialize
        //---------------------------------------------------------
        public ReadFile()
        {
            soundList = new List<SoundProperties>();
            names = new List<String>();
        }

        //---------------------------------------------------------
        // Return sound properties list
        //---------------------------------------------------------
        public List<SoundProperties> GetSoundProperties()
        {
            return soundList;
        }

        //---------------------------------------------------------
        // Return sound names
        //---------------------------------------------------------
        public List<String> GetSounds()
        {
            return names;
        }

        //---------------------------------------------------------
        // Read sound properties from file
        //---------------------------------------------------------
        public void ReadFromFile()
        {
            try
            {   // Open the text file using a stream reader.
                // When running from debug use ../../ if standalone use ../../../
#if DEBUG
                FileStream fcreate = File.Open("../../Assets/Sounds/soundConfigList.txt", FileMode.OpenOrCreate);
#else
                FileStream fcreate = File.Open("Assets/Sounds/soundConfigList.txt", FileMode.OpenOrCreate);
#endif
                using (StreamReader file = new StreamReader(fcreate))
                {
                    // Read the stream to a string, and write the string to the console.
                    String line;
                    while ((line = file.ReadLine()) != null)
                    {
                        // Extract name
                        var name = line.Substring(0, line.IndexOf(','));
                        int position = name.LastIndexOf('/');
                        // If there is no forward slash
                        // only checks forward slash
                        if (position == -1)
                        {
                            names.Add(name);
                        }
                        else
                        {
                            name = name.Substring(position + 1);
                            names.Add(name);
                        }

                        //Extract volume
                        var vol = line.Substring(line.IndexOf(',') + 1, line.Substring(line.IndexOf(',') +1).IndexOf(','));

                        //Extract pitch
                        var pitch = line.Substring(line.LastIndexOf(',')+1);
                        soundList.Add(new SoundProperties(line.Substring(0,line.IndexOf(',')), float.Parse(vol), float.Parse(pitch)));
                    }
                }
                fcreate.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine("The file could not be read:");
                Console.WriteLine(e.Message);
            }
        }
    }
}
