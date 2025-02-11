/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | https://www.php.net/license/3_01.txt                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Christoph M. Becker  <cmb@php.net>                           |
   | Based on: <https://thrysoee.dk/InsideCOM+/>                          |
   +----------------------------------------------------------------------+
 */

#include "comtest.h" // Generated by MIDL

long g_cComponents = 0;
long g_cLocks = 0;

class CDocument : public IDocument, IPersistStream
{
public:
	// IUnknown
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv);

	// IDispatch
	STDMETHODIMP GetTypeInfoCount(UINT* pCountTypeInfo);
	STDMETHODIMP GetTypeInfo(UINT iTypeInfo, LCID lcid, ITypeInfo** ppITypeInfo);
	STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId);
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams,
		VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);

	// IDocument
	STDMETHODIMP get_Content(BSTR* retvalue);
	STDMETHODIMP put_Content(BSTR newvalue);

	// IPersist
	STDMETHODIMP GetClassID(CLSID *pClassID);

	// IPersistStream
	STDMETHODIMP IsDirty( void);
	STDMETHODIMP Load(IStream *pStm);
	STDMETHODIMP Save(IStream *pStm, BOOL fClearDirty);
	STDMETHODIMP GetSizeMax(ULARGE_INTEGER *pcbSize);

	CDocument() : m_content(SysAllocString(L"")), m_cRef(1) { g_cComponents++; }
	~CDocument() { SysFreeString(m_content); g_cComponents--; }
	HRESULT Init(void);

private:
	BSTR m_content;
	ULONG m_cRef;
	ITypeInfo* m_pTypeInfo;
	BOOL m_dirty;
};

ULONG CDocument::AddRef()
{
	return ++m_cRef;
}

ULONG CDocument::Release()
{
	if (--m_cRef != 0) {
		return m_cRef;
	}
	m_pTypeInfo->Release();
	delete this;
	return 0;
}

HRESULT CDocument::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown) {
		*ppv = static_cast<IDocument*>(this);
	} else if (riid == IID_IDocument) {
		*ppv = static_cast<IDocument*>(this);
	} else if (riid == IID_IDispatch) {
		*ppv = static_cast<IDispatch*>(this);
	} else if (riid == IID_IPersistStream) {
		*ppv = static_cast<IPersistStream*>(this);
	} else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	AddRef();
	return S_OK;
}

HRESULT CDocument::GetTypeInfoCount(UINT* pCountTypeInfo)
{
	*pCountTypeInfo = 1;
	return S_OK;
}

HRESULT CDocument::GetTypeInfo(UINT iTypeInfo, LCID lcid, ITypeInfo** ppITypeInfo)
{
	(void) lcid;
	*ppITypeInfo = NULL;
	if (iTypeInfo != 0) {
		return DISP_E_BADINDEX;
	}
	m_pTypeInfo->AddRef();
	*ppITypeInfo = m_pTypeInfo;
	return S_OK;
}

HRESULT CDocument::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
{
	(void) lcid;
	if (riid != IID_NULL) {
		return DISP_E_UNKNOWNINTERFACE;
	}
	return DispGetIDsOfNames(m_pTypeInfo, rgszNames, cNames, rgDispId);
}

HRESULT CDocument::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams,
	VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
	(void) lcid;
	if (riid != IID_NULL) {
		return DISP_E_UNKNOWNINTERFACE;
	}
	return DispInvoke(this, m_pTypeInfo, dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr); 
}

HRESULT CDocument::get_Content(BSTR* retvalue)
{
	*retvalue = SysAllocString(m_content);
	return S_OK;
}

HRESULT CDocument::put_Content(BSTR newvalue)
{
	SysFreeString(m_content);
	m_content = SysAllocString(newvalue);
	return S_OK;
}

HRESULT CDocument::Init(void)
{
	ITypeLib* pTypeLib;
	if (FAILED(LoadRegTypeLib(LIBID_ComTest, 1, 0, LANG_NEUTRAL, &pTypeLib))) {
		return E_FAIL;
	}
	HRESULT hr = pTypeLib->GetTypeInfoOfGuid(IID_IDocument, &m_pTypeInfo);
	if (FAILED(hr)) {
		pTypeLib->Release();
		return hr;
	}

	pTypeLib->Release();
	return S_OK;
}

HRESULT CDocument::GetClassID(CLSID *pClassID)
{
	*pClassID = CLSID_Document;
	return S_OK;
}

HRESULT CDocument::IsDirty(void)
{
	return m_dirty ? S_OK : S_FALSE;
}

HRESULT CDocument::Load(IStream *pStm)
{
	ULONG read = 0;
	ULONG cbSize;

	if (FAILED(pStm->Read(&cbSize, sizeof cbSize, &read))) {
		return S_FALSE;
	}
	if (!SysReAllocStringLen(&m_content, NULL, cbSize)) {
		return S_FALSE;
	}
	if (FAILED(pStm->Read(m_content, cbSize, &read))) {
		// we may have garbage in m_content, but don't mind
		return S_FALSE;
	}
	m_dirty = FALSE;
	return S_OK;
}

HRESULT CDocument::Save(IStream *pStm, BOOL fClearDirty)
{
	ULONG written = 0;
	ULONG cbSize = SysStringByteLen(m_content);
	HRESULT hr;
	if (FAILED(hr = pStm->Write(&cbSize, sizeof cbSize, &written))) {
		return S_FALSE;
	}
	if (FAILED(hr = pStm->Write(m_content, cbSize, &written))) {
		return S_FALSE;
	}

	if (fClearDirty) {
		m_dirty = FALSE;
	}

	return S_OK;
}

HRESULT CDocument::GetSizeMax(ULARGE_INTEGER *pcbSize)
{
	(*pcbSize).QuadPart = sizeof(ULONG) + SysStringByteLen(m_content);
	return S_OK;
}

class CDocumentFactory : public IClassFactory
{
public:
	// IUnknown
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv);

	// IClassFactory
	STDMETHODIMP CreateInstance(IUnknown* pUnknownOuter, REFIID riid, void** ppv);
	STDMETHODIMP LockServer(BOOL bLock);

	CDocumentFactory() : m_cRef(1) { g_cLocks++; }
	~CDocumentFactory() { g_cLocks--; }

private:
	ULONG m_cRef;
};

ULONG CDocumentFactory::AddRef()
{
	return ++m_cRef;
}

ULONG CDocumentFactory::Release()
{
	if (--m_cRef != 0) {
		return m_cRef;
	}
	delete this;
	return 0;
}

HRESULT CDocumentFactory::QueryInterface(REFIID riid, void** ppv)
{
	if (riid == IID_IUnknown) {
		*ppv = (IUnknown*)this;
	} else if (riid == IID_IClassFactory) {
		*ppv = (IClassFactory*)this;
	} else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	AddRef();
	return S_OK;
}

HRESULT CDocumentFactory::CreateInstance(IUnknown *pUnknownOuter, REFIID riid, void** ppv)
{
	if (pUnknownOuter != NULL) {
		return CLASS_E_NOAGGREGATION;
	}

	CDocument *pDocument = new CDocument;
	if (pDocument == NULL) {
		return E_OUTOFMEMORY;
	}

	HRESULT hr;
	if (FAILED(hr = pDocument->Init())) {
		return hr;
	}
	hr = pDocument->QueryInterface(riid, ppv);
	pDocument->Release();
	return hr;
}

HRESULT CDocumentFactory::LockServer(BOOL bLock)
{
	if (bLock) {
		g_cLocks++;
	} else {
		g_cLocks --;
	}
	return S_OK;
}

HRESULT __stdcall DllCanUnloadNow()
{
	if (g_cLocks == 0) {
		return S_OK;
	} else {
		return S_FALSE;
	}
}

HRESULT __stdcall DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv)
{
	if (clsid != CLSID_Document) {
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	CDocumentFactory* pFactory = new CDocumentFactory;
	if (pFactory == NULL) {
		return E_OUTOFMEMORY;
	}

	// riid is probably IID_IClassFactory.
	HRESULT hr = pFactory->QueryInterface(riid, ppv);
	pFactory->Release();
	return hr;
}
