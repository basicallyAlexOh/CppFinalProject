

class GUIHandler {

    private:
        static GUIHandler* instPtr;
        GUIHandler(){}    

    public: 
        GUIHandler(const GUIHandler& obj) = delete;
        static GUIHandler* inst() {
            if(instPtr == nullptr){
                instPtr = new GUIHandler();
            }
            return instPtr;
        }

        void startGUI();

        void addTrack();
        
        void deleteTrack();

        void playPreview();

        void pausePreview();

        void close();

};