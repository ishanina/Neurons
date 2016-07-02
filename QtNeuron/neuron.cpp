#include <iostream>
#include <vector>
#include <random>

using namespace std;

class Neuron
{
    public:
        vector<int> send;
        vector<int> receive;
    public:
        int sendsize;
        int receivesize;
        double threshhold;
        double plasticity;
        vector<int> sendstrength;

        Neuron() {}

        Neuron(int sndsize,int rcvsize,double plast = 0.0,double thresh = 0.0)
        {
            int i;
            sendsize = sndsize;
            receivesize = rcvsize;
            for (i = 0; i < sendsize; i++){
                send.push_back(-1);
                sendstrength.push_back(0);
            }
            for (i = 0; i < receivesize; i++)
                receive.push_back(-1);
            plasticity = plast;
            threshhold = thresh;
        }

        int ReceiveAdd(int value)
        {
            // Connects this neuron to receive, returns index of their connection
            int i = -1,j;
            for(j = 0; j < receivesize; j++)
                if(receive[j] == value){
                    i = j;
                    break;
                }
            if (i == -1)
            {
                i = ReceiveAdd(-1);
                receive[i] = value;
            }
            return i;
        }

        int SendAdd(int value)
        {
            // Connects this neuron to send, returns index of their connection
            int i = -1,j;
            for(j = 0; j < sendsize; j++)
                if(send[j] == value){
                    i = j;
                    break;
                }
            if (i == -1)
            {
                i = SendAdd(-1);
                send[i] = value;
            }
            return i;
        }

        // Connects two neurons together
        void ReceiveFrom(Neuron* other, int thisindex, int index)
        {
            ReceiveAdd(index);
            other->SendAdd(thisindex);
        }
        void SendTo(Neuron* other, int thisindex, int index)
        {
            SendAdd(index);
            other->ReceiveAdd(thisindex);
        }
};

class InputNeuron : public Neuron
{
    public:
        double value;
        InputNeuron(int sndsize,double plast = 0.0,double val = 0) : Neuron(sndsize,0,plast,0)
        {
            value = val;
        }
};

class OutputNeuron : public Neuron
{
    public:
        double value;
        OutputNeuron(int rcvsize,double plast = 0.0,double thresh = 0.0) : Neuron(0,rcvsize,plast,thresh)
        {
            value = 0;
        }
};

class Brain
{
    void
    Think()
    {
        //int i;
        //for (i = 0; i < neuronsize; i++)
        return;
    }
    public:
        // Order is Neurons, Outputs, Inputs
        vector<Neuron*> neurons;
        vector<OutputNeuron*> outputs;
        vector<InputNeuron*> inputs;
        int NInputs;
        int NOutputs;
        int N;
        Brain(int neuronsize,int inputsize,int outputsize,int sendsize, int receivesize, double plast = 0.0, double thresh = 0.0)
        {
            int i;
            N = neuronsize;
            NInputs = inputsize;
            NOutputs = outputsize;
            for (i = 0; i < N; i++)
                neurons.push_back(new Neuron(sendsize,receivesize,plast,thresh));
            for (i = 0; i < NInputs; i++)
                inputs.push_back(new InputNeuron(sendsize,plast));
            for (i = 0; i < NOutputs; i++){
                outputs.push_back(new OutputNeuron(receivesize,plast,thresh));
            }
        }
        Neuron* GetNeuron(int index)
        {
            if (index >= 0 && index < N){
                //cout << "Neuron\n";
                return neurons[index];
            }
            else if (index >= N && index < N + NOutputs){
                return outputs[index-N];
                cout << "Output\n";
            }
            else if (index >= 0 && index < N + NOutputs + NInputs){
                return inputs[index-N-NOutputs];
                cout << "Input\n";
            }
            else{
                return nullptr;
                cout << "null\n";
            }
        }
        vector<int> ThinkStep(vector<int> input)
        {
            int i;
            for (i = 0; i < NInputs; i++)
                inputs[i]->value = input[i];
            Think();
            vector<int> output;
            for (i = 0; i < NOutputs; i++)
                output.push_back(outputs[i]->value);
            return output;
        }
};

int main()
{
    random_device rd;     // only used once to initialise (seed) engine
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> uni(0,99); // guaranteed unbiased
    Brain* brain = new Brain(100,10,10,120,120,1,0);
    int i, j;

    for (i = 0; i < 100; i++)
        for (j = 0; j < 90; j++)
        {
            int l = uni(rng);
            cout << "Connecting " << i << " to " << l << "\n";
            brain->neurons[i]->SendTo(brain->neurons[l],i,l);
        }
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
        {
            int l = uni(rng);
            cout << "Connecting Input" << i << " to " << l << "\n";
            brain->inputs[i]->SendTo(brain->neurons[l],i,l);
        }
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
        {
            int l = uni(rng);
            cout << "Connecting Output" << i << " to " << l << "\n";
            brain->outputs[i]->ReceiveFrom(brain->GetNeuron(l),i,l);
        }
    return 0;
}