using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

//-----------------------------
// By Jishnu Girish
//-----------------------------
namespace SoundEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public ObservableCollection<ComboBoxItem> cbItems { get; set; }
        public ComboBoxItem SelectedcbItem { get; set; }

        ReadFile file = new ReadFile();
        String name;
        float vol;
        float pitch;

        public MainWindow()
        {
            InitializeComponent();
            
            CreateDropDown();           // Create the drop down window
        }

        //-----------------------------------------------------
        // Create drop down window from file
        //-----------------------------------------------------
        private void CreateDropDown()
        {
            DataContext = this;
            
            file.ReadFromFile();                                                // Read from file
            cbItems = new ObservableCollection<ComboBoxItem>();                 // Create dropdown list
            var cbItem = new ComboBoxItem { Content = "<--Select-->" };
            SelectedcbItem = cbItem;
            cbItems.Add(cbItem);
            foreach (var sound in file.GetSounds())                             // Populate list    
            {
                cbItems.Add(new ComboBoxItem { Content =  sound});
            }
        }

        //----------------------------------------------------------
        // Event call back for button click
        // Updates the sound properties and write new properties to file
        //----------------------------------------------------------
        private async void Button_Click(object sender, RoutedEventArgs e)
        {
            UpdateBtn.IsEnabled = false;                                    // Disable update button
            var sounds = file.GetSoundProperties();                         // Read current sound
            int i = -1;
            foreach(var sound in sounds)                                    // Update sound properties
            {
                i++;
                var nameFromFile = sound.name;
                int position = nameFromFile.LastIndexOf('/');
                nameFromFile = nameFromFile.Substring(position + 1);
                if (nameFromFile != name)
                    continue;
                sounds[i].volume = vol;
                sounds[i].pitch = pitch;
            }

            WriteToFile(sounds);                                            // Write new sound properties to file
            await Task.Delay(1000);
            UpdateBtn.IsEnabled = true;
        }

        //-----------------------------------------------------------------
        // Event callback for volume textbox change
        // Stores copy of new value
        //-----------------------------------------------------------------
        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            var textBox = sender as TextBox;
            vol = float.Parse(textBox.Text);
        }

        //-----------------------------------------------------------------
        // Event callback for pitch textbox change
        // Stores copy of new pitch value
        //-----------------------------------------------------------------
        private void TextBox_TextChanged_1(object sender, TextChangedEventArgs e)
        {
            var textBox = sender as TextBox;
            pitch = float.Parse(textBox.Text);
        }

        //-----------------------------------------------------------------
        // Event callback for new drop down selection
        // Loads in current sound properties for the selected file
        //-----------------------------------------------------------------
        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
           name = SelectedcbItem.Content.ToString();
            int index = -1;
            foreach (var sound in file.GetSounds())         // Find the index of the selected sound
            {
                index++;                                    // Update index
                if(name == "<--Select-->")                  // if selected value is initial SELECT option 
                {                                           // set index to -1 and break
                    index = -1;
                    break;
                }
                if (sound == name)                          // Break when index is found
                {
                    break;
                }
            }
            if(index == -1)                                 // Set default values
            {
                volValue.Value = 0;
                pitchValue.Value = 0;
                return;
            }
            var sounds = file.GetSoundProperties();         
            volValue.Value = sounds[index].volume;          // Set new value for volume
            pitchValue.Value = sounds[index].pitch;         // Set new value for pitch
        }

        //-----------------------------------------------------------------
        // Write new values to file asynchronously
        //-----------------------------------------------------------------
        void WriteToFile(List<SoundProperties> sounds)
        {
            try
            {
#if DEBUG
                FileStream fcreate = File.Open("../../Assets/Sounds/soundConfigList.txt", FileMode.OpenOrCreate);   // Overwrite the file
#else
                 FileStream fcreate = File.Open("Assets/Sounds/soundConfigList.txt", FileMode.OpenOrCreate);   // Overwrite the file
#endif
                using (StreamWriter outputFile = new StreamWriter(fcreate))
                {
                    foreach (var sound in sounds)
                    {
                        var line = sound.name + "," + sound.volume + "," + sound.pitch;     // Create line in the right seqeuence
                        outputFile.WriteLineAsync(line);                                    // Write to file asynchronously
                    }
                }
                fcreate.Close();                                                            // Close when done
            }
            catch(Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
