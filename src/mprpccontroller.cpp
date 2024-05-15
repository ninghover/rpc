#include"mprpccontroller.h"

MprpcController::MprpcController()
{
    m_failed=false;
    m_errText="";
}
void MprpcController::Reset()
{
    m_failed=false;
    m_errText="";
}
bool MprpcController::Failed()const //函数不修改对象的成员
{
    return m_failed;
}
string MprpcController::ErrorText()const
{
    return m_errText;
}
void MprpcController::SetFailed(const string&reason)
{
    m_errText=true;
    m_errText=reason;
}


void MprpcController::StartCancel()
{

}
bool MprpcController::IsCanceled() const
{

}
void MprpcController::NotifyOnCancel(Closure*callback)
{

}