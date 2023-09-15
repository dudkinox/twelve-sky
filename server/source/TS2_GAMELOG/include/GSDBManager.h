#pragma once

#include <tchar.h>
#include <string>
#include <icrsint.h>

#import "msado15.dll" rename ("EOF", "EndOfFile")

/** END_ADO_BINDING과 완전히 같은 매크로.
 *
 *  @warning    namespace 문제 때문에 ADO VC extension을 GS 라이브러리와 
 *              같이 사용하기 어려운 문제를 해결하기 위해 정의한다.
 */
#define FINISH_ADO_BINDING()   {0, ADODB::adEmpty, 0, 0, 0, 0, 0, 0, 0, FALSE}};\
    return rgADOBindingEntries;}

#ifdef _DEBUG
    #pragma comment (lib, "comsuppwd.lib")
#else
    #pragma comment (lib, "comsuppw.lib")
#endif

class CDailyLog;

namespace GS {
    namespace DB {
        namespace ADO {

            #define GS_DB_ADO_VER   _T ("VER1.7.4")

            /** ADODB를 사용한 DB class.
             *
             *  History:
             *      2007-04-04      초기 버전(Ver 1.0) 작성.
             *                      Support only synchronously connection and query.
             *
             *      2007-04-05      사장님 우선 요청 사항 반영.
             *                      1. Header file 하나만 필요하도록 수정.
             *                      2. (IP, PORT, ID, PW)를 받는 Connect method 추가.
             *                      3. Disconnect method 추가.
             *                      4. Log를 default로 기록하지 않도록 수정.
             *                      5. Reconnect method 추가.
             *                      6. 로그 filter 옵션을 제거하고 모든 로그를 기록한다.
             *                      7. Query 실행 관련 코드 수정.
             *                         Execute & ExecuteAndReceive method를 호출했을때,
             *                         만약 connection이 끊어져 있다면 다시 접속을 
             *                         시도해보고 그래도 안된다면 실패를 반환한다.
             *
             *      2007-04-09      바이너리 관련 코드 추가.
             *                      1. BINARY, VARBINARY, IMAGE type support.
             *                      2. VERSION STRING 추가.
             *
             *      2007-04-10      GetResultCounts 문제 수정.
             *                      Command interface의 Execute method를 사용해서 SELECT와
             *                      같은 쿼리를 실행했을 경우 반환되는 Recordset instance
             *                      에서 GetRecordCount method를 호출해서 반환된 record의
             *                      수를 얻을 수 없는 문제가 있었다. 
             *                      해결 방법으로 Command interface 대신에 Recordset을 
             *                      사용해서 쿼리를 실행하는 것과 카운트 하는 방법 중에 
             *                      현재(VER1.5.1)는 카운트 하도록 구현.
             *
             *      2007-04-12      ADO VC exntension 기능 사용가능하도록 수정(VER1.6.0).
             *                      
             *      2007-04-18      Default로 로그 기록 폴더를 생성하지 않도록 수정.
             *                      EnableLog method를 호출했을 때 폴더를 생성합니다.
             *
             *      2007-04-24      쿼리 실행이 실패한 경우 syntax error인지 확인할 수 있는
             *                      방법 추가(VER1.6.2).
             *
             *      2007-04-24      Syntax error 추가 반영 (VER1.6.3).
             *                      1. 값이 짤리거나 넘어가는 경우.
             *                      2. NULL 값을 가질 수 없는 필드에 NULL을 INSERT 하는 경우.
             *
             *      2007-05-05      IsConnected 관련 버그 수정 (VER1.6.4)
             *                      일단 데이터베이스에 연결이 된 이후에 connection이 유지되지
             *                      않고 끊어지더라도 connection interface의 state property는
             *                      adStateOpen 상태로 유지되기 때문에 라이브러리에서 정의한
             *                      syntax 에러가 아니라면 exception handler에서 Disconnect 
             *                      method를 호출하도록 한다.
             *
             *      2007-05-05      Transaction 기능을 지원합니다(VER1.7.0).
             *                      BeginTransaction, EndTransation method 추가.
             *                      MoveFirst method에서 BOF또는 EOF exception이 발생하는 경우
             *                      1.6.4 버전 업데이트 때문에 발생하던 데이터베이스와 연결을
             *                      끊는 문제를 수정. BOF 또는 EOF exception을 syntax error로
             *                      간주하도록 IsSyntaxError method를 수정.
             *
             *      2007-05-07      에러가 발생했을 경우 network 장애와 관련된 경우만
             *                      연결을 끊도록 수정(VER1.7.1)
             *                      BOF 또는 EOF 관련 에러인 경우에도 연결을 끊게되면 이전 
             *                      버전과 같이 connect / disconnect가 반복되는 문제를 
             *                      갖게 된다.
             *
             *      2007-05-07      BEGIN TRANSACTION 쿼리 문으로 트랜잭션 기능을 구현하던 것을
             *                      connection interface의 BeginTrans, CommitTrans 그리고
             *                      RollbackTrans method를 사용하도록 수정(VER1.7.2).
             *
             *      2007-05-08      Transaction이 걸려 있는 상태에서 서버와의 연결이 끊어져도 
             *                      다시 서버가 연결 가능해졌을 때 재연결이 되도록 수정
             *                      (VER1.7.3)
             *
             *      2007-06-07      로그를 사용하지 않게 되어 있는 경우 쿼리의 결과로 
             *                      recordset이 없는 경우 disconnect를 하게 되어 있는
             *                      버그 수정 (VER1.7.4).
             *
             *  @author     ships.
             *  @ver        1.7.4
             */
            class CGSDBManager
            {
            public:
                CGSDBManager(void);
                ~CGSDBManager(void);

            public:
                /** 로그를 기록할 것인지 기록하지 않을지 결정한다.
                 *
                 *  로그를 기록할 폴더가 없는 경우에 로그를 활성화 시킬 수 없다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      bEnable     기록 - true / 기록하지 않음 - false.
                 */
                bool    EnableLog (bool bEnable);

                /** DB Manager를 사용하기 전에 반드시 호출해야 한다.
                 *
                 *  DB Manager에서 사용하는 모든 smart pointer를 생성한다.
                 *
                 *  에러 코드 :
                 *      0       -       성공인 경우.
                 *      1       -       Connection interface 생성 실패.
                 *      2       -       Recordset interface 생성 실패.
                 *      3       -       Command interface 생성 실패.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      iCode   실패했을 경우의 에러 코드.
                 *  @see        InitializeLog.
                 */
                bool    Initialize (int& iCode);

                /** Database에 연결 한다.
                 *
                 *  Connection stream(string) sample:
                 *      "Provider=sqloledb;Network Library=DBMSSOCN;Data Source=127.0.0.1, 1433;
                 *       DataBase=Gaion;UID=anonymous;PWD=blahblah;"
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      pConnectionStream   DB 연결과 관련된 string.
                 */
                bool    Connect (const TCHAR* pConnectionStream);

                /** Database에 연결한다.
                 *
                 *  Row interface로 ole db를 사용하고 tcp/ip protocol 사용하도록 fix.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      pIP     SQL server ip address.
                 *  @param      iPort   SQL server port.
                 *  @param      pID     SQL server login id.
                 *  @param      pPW     SQL server login password.
                 */
                bool    Connect (const TCHAR* pIP, const int iPort, 
                                 const TCHAR* pID, const TCHAR* pPW);

                /** 로그 파일을 기록할 경로와 파일 이름의 prefix, filter를 정해준다.
                 *
                 *  InitializeLog method를 호출하지 않으면 미리 정의된 위치에 
                 *  모든 로그를 기록한다.
                 *
                 *  History:
                 *      2007-04-05  로그 filter를 옵션에서 제외하고 모든 LOG를 
                 *                  기록하도록 변경한다.
                 *
                 *  @param      pPath       로그 파일 기록 위치.
                 *  @param      pPrefix     로그 파일 이름의 prefix.
                 *  @param      uiMask      로그 기록 대상 filter mask (deprecated).
                 *  @warning    반드시 Initialize method보다 먼저 호출해야 한다.
                 *  @warning    로그 파일을 기록할 path 생성은 recursive하지 
                 *              않기 때문에 가장 마지막 폴더명을 제외한 폴더는
                 *              반드시 이미 생성되어 있어야만 한다. 로그를 
                 *              기록할 폴더가 생성되지 않는 경우에는 EnableLog
                 *              method를 사용하더라도 로그가 기록되지 않는다.
                 *  @see        Initialize.
                 */
                void    InitializeLog (const TCHAR* pPath, const TCHAR* pPrefix);

                /** 결과를 받아야 하는 query문을 사용하는 경우 호출한다.
                 *
                 *  성공인 경우 query의 결과는 저장된다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      pQuery      Query statement.
                 *  @param      lAffected   Query에 영향을 받은 row count.
                 *  @see        Execute.
                 */
                bool    ExecuteAndReceive (const TCHAR* pQuery, int& iAffected);

                /** 결과를 받지 않는 query문을 사용하는 경우 호출한다.
                 *
                 *  결과를 받지 않는 경우에도 ExecuteAndReceive method를 호출해도
                 *  문제가 생기지 않는다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      pQuery      Query statement.
                 *  @param      lAffected   Query에 영향을 받은 row count.
                 *  @see        ExecuteAndReceive.
                 */
                bool    Execute (const TCHAR* pQuery, int& iAffected);

                /** 데이터베이스 서버에 연결되어 있는지 반환한다.
                 *
                 *  @return     연결되어 있는 경우 - true / 끊긴 경우 - false.
                 */
                bool    IsConnected (void);

                /** Query에 따라 영향을 받은 row count를 반환한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      iCounts     Row counts number.
                 *  @warning    Query 실행 결과를 받은 후 Recordset pointer를
                 *              이동하면서 어떤 작업을 한 후에 GetResultCounts
                 *              method를 호출하면 작업 중이던 pointer로 돌아갈
                 *              수 없기 때문에 값이 필요한 경우에는 반드시 
                 *              query 실행 후 제일 먼저 호출하도록 한다.
                 */
                bool    GetResultCounts (int& iCounts);

                /** Recordset의 다음 row로 pointer를 이동합니다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 */
                bool    Forward (void);

                /** Recordset의 제일 처음 row로 pointer를 이동합니다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 */
                bool    MoveFirst (void);

                /** Recordset의 지정된 위치의 row로 pointer를 이동합니다.
                 *
                 *  현재 recordset pointer를 주어진 숫자만큼 foward 방향으로 
                 *  이동합니다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      iPos    옮길 row의 수.
                 */
                bool    Move (int iPos);

                /** 현재 recordset의 pointer가 마지막을 가리키고 있는지 반환한다.
                 *
                 *  @return     마지막 - true / 그렇지 않은 경우 - false.
                 *  @warning    Recordset pointer의 상태가 ADODB::adStateOpen이 
                 *              아닌 경우에도 recordset이 없으므로 마지막이라고
                 *              간주하고 true를 반환한다.
                 */
                bool    IsEndOfRecordset (void);

                /** Date type의 record 자료를 반환한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      iField      Table field index.
                 *  @param      datetime    Data result.
                 */
                bool    GetDate (int iField, DATE& datetime);

                /** Int type의 record 자료를 반환한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      iField      Table field index.
                 *  @param      iValue      Data result.
                 */
                bool    GetInt (int iField, int& iValue);

                /** Int type의 record 자료를 반환한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      pFieldName  Table field name.
                 *  @param      iValue      Data result.
                 */
                bool    GetInt (TCHAR* pFieldName, int& iValue);

                /** string type의 record 자료를 반환한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      iField      Table field index.
                 *  @param      pValue      Data result.
                 *  @param      iLength     입력 버퍼의 길이.
                 */
                bool    GetText (int iField, TCHAR* pValue, int iLength);

                /** Long type의 record 자료를 반환한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      iField      Table field index.
                 *  @param      lValue      Data result.
                 */
                bool    GetLong (int iField, long& lValue);

                /** Float type의 record 자료를 반환한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      iField      Table field index.
                 *  @param      fValue      Data result.
                 */
                bool    GetFloat (int iField, float& fValue);

                /** Double type의 record 자료를 반환한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      iField      Table field index.
                 *  @param      dValue      Data result.
                 */
                bool    GetDouble (int iField, double& dValue);

                //bool    GetBool (int iField, bool& bValue);

                /** Database에 연결을 끊는다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 */
                bool    Disconnect (void);

                /** Database 서버에 재접속을 한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 */
                bool    Reconnect (void);

                /** 바이너리 파라미터의 값을 설정한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      pChunk      바이너리 field에 저장될 바이너리 데이타.
                 *  @param      dwChunkSize 바이너리 데이타 크기.
                 */
                bool    PushBinaryParameter (const TCHAR* pName, LPVOID pChunk, DWORD dwChunkSize);

                /** Parameter가 있는 query문을 준비한다.
                 *
                 *  Prepared property를 사용해서 데이타베이스 서버에서 query문 
                 *  parsing을 최소화할 수 있도록 한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      pQuery      SQL query.
                 *  @param      bPrepare    Prepared property.
                 */
                bool    PrepareStatement (const TCHAR* pQuery, bool bPrepare = false);

                /** 파라미터가 있는 query문을 실행한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      iAffected   Query에 영향을 받은 row count.
                 *  @param      bIsResult   결과를 받는 경우 - true / false.
                 */
                bool    Execute (int& iAffected, bool bIsResult = false);

                /** 바이너리 필드의 값을 반환한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      iField      Table field index.
                 *  @param      dwChunkSize Binary data size.
                 *  @param      pChunk      Binary data raw stream.
                 */
                bool    GetBinary (int iField, DWORD& dwChunkSize, LPVOID& pChunk);

                /** Database table과 class를 서로 연결한다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      pInstance   Inherited CADORecordBinding instance.
                 */
                bool    Bind (void* pInstance);

                /** Transaction을 시작합니다.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @see        EndTransaction.
                 */
                bool    BeginTransaction (void);

                /** Transaction을 종료합니다.
                 *
                 *  복구 여부:
                 *      true    -   transaction commit.
                 *      false   -   transaction rollback.
                 *
                 *  @return     성공 - true / 실패 - false.
                 *  @param      bIsCommit   복구 여부.
                 *  @see        BeginTransaction.
                 */
                bool    EndTransaction (bool bIsCommit = true);

            private:
                void    WriteProviderErrorLog (ADODB::_ConnectionPtr pConn);
                void    WriteComErrorLog (_com_error& e, bool bExit = true);
                bool    IsOpenRecordset (void);

            protected:
                ADODB::_ConnectionPtr   m_pConnection;      ///< Connection smart pointer.
                ADODB::_RecordsetPtr    m_pRecordset;       ///< Recordset smart pointer.
                ADODB::_CommandPtr      m_pCommand;         ///< Command smart pointer.
                CDailyLog*              m_pLogger;           ///< Log instance.
                bool                    m_bWriteLog;        ///< 로그 기록 여부.
                bool                    m_bInitializedLog;  ///< 로그 경로 설정 여부.
                bool                    m_bLoggerCreated;   ///< 로그 기록 가능 여부.
                std::string             m_ConnectionStream; ///< Connection stream string.
                IADORecordBinding*      m_pBinder;
                std::string             m_LogPath;
                HRESULT                 m_hError;
                bool                    m_bIsTransactionStart;
                bool                    m_bIsTransactionFailed;
            };

        } // End Of ADO.
    }// End Of DB.
} // End Of GS.