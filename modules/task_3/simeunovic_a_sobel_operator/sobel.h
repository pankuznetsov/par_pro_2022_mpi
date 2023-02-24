#include<vector>
void CreateRandomImage(std::vector<int>& image, int h, int w);
void CalculateSober(std::vector<int>& image, int h, int w, std::vector<int>& resultX, std::vector<int>& resultY);
void DataReceiving(std::vector<int>& pProcResultX, std::vector<int>& pProcResultY, int h, int w, std::vector<int>& resultX, std::vector<int>& resultY);
void ParallelCalculation(std::vector<int>& image,int h,int w,int StartRow,int RowNum,std::vector<int>& pProcResultX,std::vector<int>& pProcResultY);
void SequentalCalculation(std::vector<int>& image, int h, int w, std::vector<int>& SeqResultX,std::vector<int>& SeqResultY);
