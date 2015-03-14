// GuiDoc.h : Schnittstelle der Klasse CGuiDoc
//


#pragma once

class CGuiDoc : public CDocument
{
protected: // Nur aus Serialisierung erstellen
	CGuiDoc();
	DECLARE_DYNCREATE(CGuiDoc)

// Attribute
public:

// Operationen
public:

// Überschreibungen
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementierung
public:
	virtual ~CGuiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	DECLARE_MESSAGE_MAP()
};


