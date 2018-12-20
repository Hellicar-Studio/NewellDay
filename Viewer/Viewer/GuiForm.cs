using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.IO;
using System.Dynamic;

namespace Viewer
{
    public class Settings
    {
        public static int XPosition { set; get; }
        public static int YPosition { set; get; }
        public static int Width { set; get; }
        public static int Height { set; get; }

    }
    public partial class GuiForm : Form
    {
        private static ViewerForm mf;
        //JsonTextReader reder = new JsonTextReader(n)

        public GuiForm(ViewerForm mainForm)
        {
            mf = mainForm;
            InitializeComponent();
            //XPositionField.ValueChanged += new System.EventHandler(mf.OnParameterChanged);
            //YPositionField.ValueChanged += new System.EventHandler(mf.OnParameterChanged);
            //WidthField.ValueChanged += new System.EventHandler(mf.OnParameterChanged);
            //HeightField.ValueChanged += new System.EventHandler(mf.OnParameterChanged);

            XPositionField.ValueChanged += new System.EventHandler(this.GuiForm_SaveSettings);
            YPositionField.ValueChanged += new System.EventHandler(this.GuiForm_SaveSettings);
            WidthField.ValueChanged += new System.EventHandler(this.GuiForm_SaveSettings);
            HeightField.ValueChanged += new System.EventHandler(this.GuiForm_SaveSettings);

            using (StreamReader r = new StreamReader("gui.json"))
            {
                string json = r.ReadToEnd();
                JObject settings = JObject.Parse(json);
                Console.Write(settings);
                int height = Convert.ToInt32(settings["Camera:0"]["Height"]);
                //Console.WriteLine("Converted Height: " + height);
                //JsonTextReader reader = new JsonTextReader(new StringReader(json));
            }

            //XPosition.Value = Properties.Settings.Default.XPosition;
            //YPosition.Value = Properties.Settings.Default.YPosition;
            //Width.Value = Properties.Settings.Default.Width;
            //Height.Value = Properties.Settings.Default.Height;

            //trackBar1.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            //trackBar1.Scroll += new System.EventHandler(mf.trackBar1_Scroll);
        }

        private void GuiForm_SaveSettings(object sender, EventArgs e)
        {
            //Properties.Settings.Default.Save();
            //Console.WriteLine("Saving!");
        }

        private void GuiForm_Load(object sender, EventArgs e)
        {

        }
    }
}


