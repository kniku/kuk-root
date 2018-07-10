import { TestBed, inject } from '@angular/core/testing';

import { KontoService } from './konto.service';

describe('KontoService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [KontoService]
    });
  });

  it('should be created', inject([KontoService], (service: KontoService) => {
    expect(service).toBeTruthy();
  }));
});
